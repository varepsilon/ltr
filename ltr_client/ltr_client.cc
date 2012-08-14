// Copyright 2012 Yandex

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "boost/algorithm/string.hpp"

#include "logog/logog.h"

#include "ltr/data/utility/io_utility.h"

#include "ltr/learners/learner.h"
#include "ltr/learners/best_feature_learner/best_feature_learner.h"

#include "ltr/measures/ndcg.h"

#include "ltr_client/ltr_client.h"
#include "ltr_client/configurator.h"
#include "ltr_client/factory.h"
#include "ltr_client/utility/parameterized_info.h"

using std::cout;
using std::find;
using std::logic_error;
using std::string;
using std::vector;
using std::endl;
using std::ofstream;

using ltr::io_utility::loadDataSet;
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
    for (TXmlTokenSpecList::const_iterator it = spec->dependency_specs().begin();
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
void LtrClient::executeTrain(Parameterized* parameterized,
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

  cout << "\n\n\nvoid doLaunch\n";
  learner->learn(data_set);

  cout << "\n\nTrain " << data_info.file << " finished. Report:"
       << learner->report() << endl;

  for (boost::unordered_set<string>::const_iterator predict_it =
       train_info.predicts.begin();
       predict_it != train_info.predicts.end();
       ++predict_it) {
    const string& predict = *predict_it;
    if (configurator_.dataInfos().find(predict) ==
        configurator_.dataInfos().end()) {
      cout << "Can't predict. Unknown data " << predict << endl;
      return;
    }
    const string& predict_file_path = configurator_.rootPath() +
      learner_info.get_name() + "." + predict + ".predicts";

    Scorer::Ptr scorer = learner->make();

    ltr::io_utility::savePredictions(data_set, scorer, predict_file_path);

    cout << "\nsaved predictions for '" << predict
         << "' into " << predict_file_path << endl;

    if (train_info.gen_cpp) {
      string cpp_file_path = configurator_.rootPath() +
        learner_info.get_name() + ".cpp";
        ofstream fout(cpp_file_path.c_str());
        fout << scorer->generateCppCode(learner_info.get_name());
        fout.close();
        cout << "cpp code saved into " << cpp_file_path << endl;
    }
  }
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

    cout << "\nvoid LtrClient::launch()\nparameters="
         << parameters.toString() << endl;

    Parameterized* parameterized = Factory::instance()->
      Create(learner_info.get_type() + learner_info.get_approach(), parameters);

    if (learner_info.get_approach() == "listwise") {
      executeTrain<ObjectList>(parameterized, train_info);
    } else if (learner_info.get_approach() == "pairwise") {
      executeTrain<ObjectPair>(parameterized, train_info);
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
