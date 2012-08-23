// Copyright 2012 Yandex

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "boost/algorithm/string.hpp"

#include "logog/logog.h"

#include "ltr/crossvalidation/crossvalidator.h"
#include "ltr/crossvalidation/k_fold_simple_splitter.h"

#include "ltr/data/utility/io_utility.h"

#include "ltr/learners/learner.h"
#include "ltr/learners/best_feature_learner/best_feature_learner.h"

#include "ltr/measures/measure.h"
#include "ltr/measures/dcg.h"
#include "ltr/measures/ndcg.h"
#include "ltr/measures/abs_error.h"

#include "ltr_client/ltr_client.h"
#include "ltr_client/configurator.h"
#include "ltr_client/factory.h"

using std::cout;
using std::find;
using std::logic_error;
using std::string;
using std::vector;
using std::endl;
using std::ofstream;

using ltr::Parameterized;
using ltr::ParametersContainer;
using ltr::Learner;
using ltr::DataSet;
using ltr::Scorer;
using ltr::ObjectList;
using ltr::ObjectPair;
using ltr::Log;
using ltr::BestFeatureLearner;
using ltr::NDCG;
using ltr::DCG;

using ltr::io_utility::loadDataSet;

typedef  string ParameterizedDependency;

static bool BuildObjectCreationChain(const ParametrizedInfo* spec,
  TXmlTokenSpecList* queue, TXmlTokenSpecList* circularity_check_queue) {
    if (find(queue->begin(), queue->end(), spec) != queue->end()) {
      return true;
    }

    // check for circular dependencies
    TXmlTokenSpecList::const_iterator begin_it = find(
      circularity_check_queue->begin(), circularity_check_queue->end(), spec);

    if (begin_it != circularity_check_queue->end()) {
      throw logic_error("Circular dependency detected!");
    }
    circularity_check_queue->push_back(spec);

    // add dependencies
    for (TXmlTokenSpecList::const_iterator it =
         spec->dependency_specs().begin();
         it != spec->dependency_specs().end();
         ++it) {
      const ParametrizedInfo* dep_spec = *it;
      if (!BuildObjectCreationChain(dep_spec, queue, circularity_check_queue)) {
        throw logic_error("Can not resolve dependencies!");
      }
    }

    // add self
    queue->push_back(spec);
    return true;
}

static TXmlTokenSpecList values(const ConfigParser::ParameterizedInfos& cont) {
  TXmlTokenSpecList values;
  for (ConfigParser::ParameterizedInfos::const_iterator it = cont.begin();
       it != cont.end();
       ++it) {
    const ParametrizedInfo& spec = it->second;
    values.push_back(&spec);
  }
  return values;
}

TXmlTokenSpecList LtrClient::getLoadQueue() const {
  const TXmlTokenSpecList& all_specs = values(configurator_.xmlTokenSpecs());

  TXmlTokenSpecList result;
  for (TXmlTokenSpecList::const_iterator it = all_specs.begin();
       it != all_specs.end();
       ++it) {
    const ParametrizedInfo* spec = *it;
    TXmlTokenSpecList circularity_check_queue;
    BuildObjectCreationChain(spec, &result, &circularity_check_queue);
  }
  return result;
}

LtrClient::LtrClient()
: configurator_() {}

LtrClient::~LtrClient() {}

static ParametersContainer Create(
  const ParametersContainer& src_parameters,
  const ConfigParser::ParameterizedInfos& all_specs);

static Parameterized* Create(const string& name,
  const ConfigParser::ParameterizedInfos& all_specs) {
    ConfigParser::ParameterizedInfos::const_iterator it = all_specs.find(name);
    assert(it != all_specs.end());
    const ParametrizedInfo* spec = &it->second;
    const ParametersContainer& parameters =
      Create(spec->get_parameters(), all_specs);
    return Factory::instance()->Create(spec->get_type(), parameters);
}

static ParametersContainer Create(
  const ParametersContainer& src_parameters,
  const ConfigParser::ParameterizedInfos& all_specs) {
    ParametersContainer result;
    for (ParametersContainer::StringAnyHash::const_iterator it =
         src_parameters.begin();
         it != src_parameters.end();
         ++it) {
      const boost::any& parameter = it->second;
      const string& name = it->first;

      if (const ParameterizedDependency* dependency =
          boost::any_cast<ParameterizedDependency>(&parameter)) {
        typedef vector<string> TStringVector;
        TStringVector strings;
        boost::split(strings, *dependency,
                     boost::is_any_of("\t "));
        assert(strings.size());
        if (strings.size() == 1) {
          result.AddNew(name, Create(strings[0], all_specs));
        } else {
          vector<boost::any> list;
          for (TStringVector::const_iterator str_it = strings.begin();
               str_it != strings.end();
               ++str_it) {
            list.push_back(Create(*str_it, all_specs));
          }
          result.AddNew(name, list);
        }
      } else if (const ParametersContainer* cont =
                 boost::any_cast<ParametersContainer>(&parameter)) {
        result.AddNew(name, Create(*cont, all_specs));
      } else {
        result.AddNew(name, parameter);
      }
    }
    return result;
}

void LtrClient::initFrom(const string& file_name) {
  configurator_.parseConfig(file_name);
  getLoadQueue();  // check absence of circularity dependencies
}

template <class TElement>
void LtrClient::launchTrain(Parameterized* parameterized,
                            const TrainLaunchInfo& train_info) {
  Learner<TElement>* learner =
    dynamic_cast<Learner<TElement>*>(parameterized);  //NOLINT
  assert(learner);

  const ParametrizedInfo& learner_info =
    configurator_.findLearner(train_info.learner);

  const DataInfo& data_info = configurator_.findData(train_info.data);

  if (learner_info.get_approach() != data_info.approach) {
    throw logic_error("Approach of learner and data does not coincide!");
  }

  const DataSet<TElement>& data_set =
    loadDataSet<TElement>(data_info.file, data_info.format);

  INFO("\n\n\nvoid doLaunch\n");
  learner->learn(data_set);

  INFO("\n\nTrain %s finished. Report:%s\n", data_info.file.c_str(),
       learner->report().c_str());

  for (boost::unordered_set<string>::const_iterator predict_it =
       train_info.predicts.begin();
       predict_it != train_info.predicts.end();
       ++predict_it) {
    const string& predict = *predict_it;
    if (configurator_.dataInfos().find(predict) ==
        configurator_.dataInfos().end()) {
      INFO("Can't predict. Unknown data %s\n", predict.c_str());
      return;
    }
    const string& predict_file_path = configurator_.rootPath() +
      learner_info.get_name() + "." + predict + ".predicts";

    Scorer::Ptr scorer = learner->make();

    ltr::io_utility::savePredictions(data_set, scorer, predict_file_path);

    INFO("\nsaved predictions for '%s' into %s\n", predict.c_str(),
         predict_file_path.c_str());

    if (train_info.gen_cpp) {
      string cpp_file_path = configurator_.rootPath() +
        learner_info.get_name() + ".cpp";
        ofstream fout(cpp_file_path.c_str());
        fout << scorer->generateCppCode(learner_info.get_name());
        fout.close();
        INFO("cpp code saved into %s\n", cpp_file_path.c_str());
    }
  }
}

template <class TElement>
void LtrClient::launchCrossvalidation(
  const CrossvalidationLaunchInfo& crossvalidation_info) {
    boost::unordered_set<string>::const_iterator
      learners_iterator = crossvalidation_info.learners.begin(),
      measures_iterator = crossvalidation_info.measures.begin(),
      datas_iterator = crossvalidation_info.datas.begin();

    ltr::cv::CrossValidator<TElement> cross_validator;

    for (; learners_iterator != crossvalidation_info.learners.end();
         ++learners_iterator) {
      const ParametrizedInfo& learner_info =
        configurator_.findLearner(*learners_iterator);
      const ParametersContainer& parameters =
        Create(learner_info.get_parameters(), configurator_.xmlTokenSpecs());
      Parameterized* learner = Factory::instance()->Create(
        learner_info.get_type() + learner_info.get_approach(), parameters);
      cross_validator.addLearner(dynamic_cast<Learner<TElement>*>(learner)); // NOLINT
    }

    // It will work when measures will be added.
    /* for (; measures_iterator != crossvalidation_info.measures.end();
            ++measures_iterator) {
      const ParametrizedInfo& measure_info =
        configurator_.findMeasure(*measures_iterator);
      const ParametersContainer& parameters =
        Create(measure_info.get_parameters(), configurator_.xmlTokenSpecs());
      Parameterized* measure = Factory::instance()->
        Create(measure_info.get_type() + measure_info.get_approach(), parameters);
      cross_validator.addMeasure(dynamic_cast<Measure<TElement>*>(measure)); // NOLINT
    }*/

    DCG::Ptr measure = new DCG;
    cross_validator.addMeasure(measure);

    typename ltr::cv::KFoldSimpleSplitter<TElement>::Ptr splitter =
      new ltr::cv::KFoldSimpleSplitter<TElement>;
    cross_validator.addSplitter(splitter);

    for (; datas_iterator != crossvalidation_info.datas.end();
         ++datas_iterator) {
      const DataInfo& data_info = configurator_.findData(*datas_iterator);
      typename DataSet<TElement>::Ptr data_set_ptr = new DataSet<TElement>;
      *data_set_ptr = loadDataSet<TElement>(data_info.file, data_info.format);
      cross_validator.addDataSet(data_set_ptr);
    }

    cross_validator.launch();
    INFO(cross_validator.toString().c_str());
}

void LtrClient::launch() {
  for (ConfigParser::TrainInfos::const_iterator it =
       configurator_.trainInfos().begin();
       it != configurator_.trainInfos().end();
       ++it) {
    const TrainLaunchInfo& train_info = it->second;
    const ParametrizedInfo& learner_info =
      configurator_.findLearner(train_info.learner);

    const ParametersContainer& parameters =
      Create(learner_info.get_parameters(), configurator_.xmlTokenSpecs());

    INFO("\nvoid LtrClient::launch()\nparameters=%s\n",
         parameters.toString().c_str());

    Parameterized* parameterized = Factory::instance()->
      Create(learner_info.get_type() + learner_info.get_approach(), parameters);

    if (learner_info.get_approach() == "listwise") {
      launchTrain<ObjectList>(parameterized, train_info);
    } else if (learner_info.get_approach() == "pairwise") {
      launchTrain<ObjectPair>(parameterized, train_info);
    } else {
      assert(false && "Not implemented yet");
    }
  }

  for (ConfigParser::CrossvalidationInfos::const_iterator iterator =
       configurator_.crossvalidationInfos().begin();
       iterator != configurator_.crossvalidationInfos().end();
       ++iterator) {
    const CrossvalidationLaunchInfo& crossvalidation_info =
      iterator->second;
    const ParametrizedInfo& learner_info =
      configurator_.findLearner(*crossvalidation_info.learners.begin());

    if (learner_info.get_approach() == "listwise") {
      launchCrossvalidation<ObjectList>(crossvalidation_info);
      // We can't do that while we don't have measure for ObjectPair.
      // } else if (learner_info.getApproach() == "pairwise") {
      //   launchCrossvalidation<ObjectPair>(crossvalidation_info);
    } else {
      assert(false && "Not implemented yet");
    }
  }
}

// #define LOGOG_GROUP NULL

// ===========================================================================

int main(int argc, char* argv[]) {
  Log LOG;

  if (argc < 2) {
      ERR("config file  missing");
      return 1;
  }

  Factory factory;

  factory.registerType<BestFeatureLearner<ObjectList> >
    ("BEST_FEATURElistwise");

  factory.registerType<NDCG>("NDCG");

  LtrClient client;

  try {
      client.initFrom(argv[1]);
      client.launch();
  } catch(const logic_error& err) {
      ERR("Failed: %s", err.what());
  } catch(...) {
      ERR("Caught exception");
  }
  return 0;
}
