// Copyright 2012 Yandex

#include "rlog/rlog_default.h"

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "boost/algorithm/string.hpp"

#include "ltr/predictions_aggregators/predictions_aggregator.h"
#include "ltr/predictions_aggregators/average_predictions_aggregator.h"
#include "ltr/predictions_aggregators/vote_predictions_aggregator.h"

#include "ltr/crossvalidation/splitter.h"
#include "ltr/crossvalidation/crossvalidator.h"
#include "ltr/crossvalidation/k_fold_simple_splitter.h"

#include "ltr/data/utility/io_utility.h"

#include "ltr/learners/learner.h"
#include "ltr/learners/best_feature_learner/best_feature_learner.h"
#include "ltr/learners/linear_learner/linear_learner.h"
#include "ltr/learners/nearest_neighbor_learner/nearest_neighbor_learner.h"

#include "ltr/measures/measure.h"
#include "ltr/measures/dcg.h"
#include "ltr/measures/ndcg.h"
#include "ltr/measures/abs_error.h"

#include "ltr/metrics/metric.h"
#include "ltr/metrics/euclidean_metric.h"

#include "ltr_client/ltr_client.h"
#include "ltr_client/configurator.h"
#include "ltr_client/factory.h"
#include "ltr_client/registration.h"

#include "ltr/utility/neighbor_weighter.h"
#include "ltr/utility/boost/path.h"

#include "contrib/getopt_pp/getopt_pp.h"

using std::cout;
using std::cerr;
using std::find;
using std::logic_error;
using std::string;
using std::vector;
using std::endl;
using std::ofstream;

using ltr::PredictionsAggregator;
using ltr::AveragePredictionsAggregator;
using ltr::Parameterized;
using ltr::ParametersContainer;
using ltr::Learner;
using ltr::DataSet;
using ltr::Scorer;
using ltr::ObjectList;
using ltr::ObjectPair;
using ltr::BestFeatureLearner;
using ltr::LinearLearner;
using ltr::EuclideanMetric;
using ltr::Measure;
using ltr::NNLearner;
using ltr::NDCG;
using ltr::DCG;

using ltr::cv::Splitter;
using ltr::cv::KFoldSimpleSplitter;
using ltr::io_utility::loadDataSet;
using ltr::utility::InverseLinearDistance;
using ltr::utility::AppendTrailingPathSeparator;

using GetOpt::GetOpt_pp;
using GetOpt::Option;
using GetOpt::OptionPresent;
using GetOpt::GetOptEx;
using GetOpt::TooManyOptionsEx;

using ltr::LOG;

typedef  string ParameterizedDependency;

static bool BuildObjectCreationChain(const ParameterizedInfo* spec,
    ParameterizedInfosList* queue,
    ParameterizedInfosList* circularity_check_queue) {
  if (find(queue->begin(), queue->end(), spec) != queue->end()) {
    return true;
  }

  // check for circular dependencies
  ParameterizedInfosList::const_iterator begin_it = find(
    circularity_check_queue->begin(), circularity_check_queue->end(), spec);

  if (begin_it != circularity_check_queue->end()) {
    throw logic_error("circular dependency detected");
  }
  circularity_check_queue->push_back(spec);

  // add dependencies
  for (ParameterizedInfosList::const_iterator it =
        spec->dependency_specs().begin();
        it != spec->dependency_specs().end();
        ++it) {
    const ParameterizedInfo* dep_spec = *it;
    if (!BuildObjectCreationChain(dep_spec, queue, circularity_check_queue)) {
      throw logic_error("can not resolve dependencies");
    }
  }

  // add self
  queue->push_back(spec);
  return true;
}

static ParameterizedInfosList values(
    const ConfigParser::ParameterizedInfos& cont) {
  ParameterizedInfosList values;
  for (ConfigParser::ParameterizedInfos::const_iterator it = cont.begin();
       it != cont.end();
       ++it) {
    const ParameterizedInfo& spec = it->second;
    values.push_back(&spec);
  }
  return values;
}

ParameterizedInfosList LtrClient::getLoadQueue() const {
  const ParameterizedInfosList& all_specs =
    values(configurator_.xmlTokenSpecs());

  ParameterizedInfosList result;
  for (ParameterizedInfosList::const_iterator it = all_specs.begin();
       it != all_specs.end();
       ++it) {
    const ParameterizedInfo* spec = *it;
    ParameterizedInfosList circularity_check_queue;
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

static Any Create(const string& name,
  const ConfigParser::ParameterizedInfos& all_specs) {
    ConfigParser::ParameterizedInfos::const_iterator it = all_specs.find(name);
    assert(it != all_specs.end());
    const ParameterizedInfo* spec = &it->second;
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
      Any& parameter = const_cast<Any&>(it->second);
      const string& name = it->first;

      if (const ParameterizedDependency* dependency =
          any_cast<ParameterizedDependency>(&parameter)) {
        typedef vector<string> TStringVector;
        TStringVector strings;
        boost::split(strings, *dependency,
                     boost::is_any_of("\t "));
        assert(strings.size());
        if (strings.size() == 1) {
          result.AddNew(name, Create(strings[0], all_specs));
        } else {
          vector<Any> list;
          for (TStringVector::const_iterator str_it = strings.begin();
               str_it != strings.end();
               ++str_it) {
            list.push_back(Create(*str_it, all_specs));
          }
          result.AddNew(name, list);
        }
      } else if (const ParametersContainer::Ptr cont =
                 any_cast<ParametersContainer>(&parameter)) {
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
void LtrClient::launchTrain(Any parameterized,
                            const TrainLaunchInfo& train_info) {
  typename Learner<TElement>::Ptr learner =
    any_cast<typename Learner<TElement>::Ptr>(parameterized);  //NOLINT
  assert(learner);

  const ParameterizedInfo& learner_info =
    configurator_.findParameterized(train_info.learner);

  const DataInfo& data_info = configurator_.findData(train_info.data);

  if (learner_info.get_approach() != data_info.approach) {
    throw logic_error("approaches of learner and data do not coincide");
  }

  const DataSet<TElement>& data_set =
    loadDataSet<TElement>(data_info.file, data_info.format);

  rInfo("\n\nvoid doLaunch\n");
  learner->learn(data_set);

  rInfo("\n\nTrain %s finished. Report:%s\n", data_info.file.c_str(),
       learner->report().c_str());

  rInfo("\n\nStarted predicting\n");
  for (boost::unordered_set<string>::const_iterator predict_it =
       train_info.predicts.begin();
       predict_it != train_info.predicts.end();
       ++predict_it) {
    const string& predict = *predict_it;
    if (configurator_.dataInfos().find(predict) ==
        configurator_.dataInfos().end()) {
      rInfo("Can't predict. Unknown data %s\n", predict.c_str());
      return;
    }
    const string& predict_file_path =
      AppendTrailingPathSeparator(configurator_.rootPath()) +
      learner_info.get_name() + "." + predict + ".predicts";

    Scorer::Ptr scorer = learner->make();

    ltr::io_utility::savePredictions(data_set, scorer, predict_file_path);

    rInfo("\nsaved predictions for '%s' into %s\n", predict.c_str(),
         predict_file_path.c_str());

    if (train_info.gen_cpp) {
      string cpp_file_path =
        AppendTrailingPathSeparator(configurator_.rootPath()) +
        learner_info.get_name() + ".cpp";
      ofstream fout(cpp_file_path.c_str());
      fout << scorer->generateCppCode(learner_info.get_name());
      fout.close();
      rInfo("cpp code saved into %s\n", cpp_file_path.c_str());
    }
  }
}

template <class TElement>
void LtrClient::launchCrossvalidation(
  const CrossvalidationLaunchInfo& crossvalidation_info) {
    boost::unordered_set<string>::const_iterator
      learners_alias = crossvalidation_info.learners.begin(),
      measures_alias = crossvalidation_info.measures.begin(),
      datas_alias = crossvalidation_info.datas.begin();

    ltr::cv::CrossValidator<TElement> cross_validator;

    for (; learners_alias != crossvalidation_info.learners.end();
         ++learners_alias) {
      const ParameterizedInfo& learner_info =
        configurator_.findParameterized(*learners_alias);
      const ParametersContainer& parameters =
        Create(learner_info.get_parameters(), configurator_.xmlTokenSpecs());
      Any learner = Factory::instance()->Create(
        learner_info.get_type() + learner_info.get_approach(), parameters);
      cross_validator.add_learner(
        any_cast<typename Learner<TElement>::Ptr>(learner));
    }

    for (; measures_alias != crossvalidation_info.measures.end();
            ++measures_alias) {
      const ParameterizedInfo& measure_info =
        configurator_.findParameterized(*measures_alias);
      const ParametersContainer& parameters =
        Create(measure_info.get_parameters(), configurator_.xmlTokenSpecs());
      Any measure = Factory::instance()->
        Create(measure_info.get_type() + measure_info.get_approach(), parameters); // NOLINT
      cross_validator.add_measure(any_cast<typename Measure<TElement>::Ptr>(measure)); // NOLINT
    }

    const ParameterizedInfo& splitter_info =
      configurator_.findParameterized(crossvalidation_info.splitter);
    const ParametersContainer& parameters =
      Create(splitter_info.get_parameters(), configurator_.xmlTokenSpecs());
    Any splitter = Factory::instance()->
      Create(splitter_info.get_type() + splitter_info.get_approach(), parameters); // NOLINT
    cross_validator.add_splitter(any_cast<typename Splitter<TElement>::Ptr>(splitter)); // NOLINT

    for (; datas_alias != crossvalidation_info.datas.end();
         ++datas_alias) {
      const DataInfo& data_info = configurator_.findData(*datas_alias);
      DataSet<TElement> data_set = DataSet<TElement>();
      data_set = loadDataSet<TElement>(data_info.file, data_info.format);
      cross_validator.add_data_set(data_set);
    }

    cross_validator.launch();
    rInfo(cross_validator.toString().c_str());
}

void LtrClient::launch() {
  for (ConfigParser::TrainInfos::const_iterator it =
       configurator_.trainInfos().begin();
       it != configurator_.trainInfos().end();
       ++it) {
    const TrainLaunchInfo& train_info = it->second;
    const ParameterizedInfo& learner_info =
      configurator_.findParameterized(train_info.learner);

    const ParametersContainer& parameters =
      Create(learner_info.get_parameters(), configurator_.xmlTokenSpecs());

    rInfo("\nvoid LtrClient::launch()\nparameters=%s\n",
         parameters.toString().c_str());

    Any parameterized = Factory::instance()->
      Create(learner_info.get_type() + learner_info.get_approach(), parameters);

    if (learner_info.get_approach() == "listwise") {
      launchTrain<ObjectList>(parameterized, train_info);
    } else if (learner_info.get_approach() == "pairwise") {
      launchTrain<ObjectPair>(parameterized, train_info);
    } else if (learner_info.get_approach() == "pointwise") {
      launchTrain<Object>(parameterized, train_info);
    }
  }

  for (ConfigParser::CrossvalidationInfos::const_iterator iterator =
       configurator_.crossvalidationInfos().begin();
       iterator != configurator_.crossvalidationInfos().end();
       ++iterator) {
    const CrossvalidationLaunchInfo& crossvalidation_info =
      iterator->second;
    const ParameterizedInfo& learner_info =
      configurator_.findParameterized(*crossvalidation_info.learners.begin());

    if (learner_info.get_approach() == "listwise") {
      launchCrossvalidation<ObjectList>(crossvalidation_info);
    }

    if (learner_info.get_approach() == "pairwise") {
      launchCrossvalidation<ObjectPair>(crossvalidation_info);
    }

    if (learner_info.get_approach() == "pointwise") {
      launchCrossvalidation<Object>(crossvalidation_info);
    }
  }
}


// ===========================================================================

void show_usage() {
  cout << "Usage: ltr_client -f <configfilename> [-v | -h]" << endl;
  cout << "Executes commands defined in xml config file." << endl << endl;
  cout << "Options:" << endl;
  cout << "  -f,  filename\t\tconfig file name" << endl;
  cout << "  -h,  help    \t\tdisplay this help and exit" << endl;
  cout << "  -v,  verbose \t\tproduce additional output to console" << endl;
}

int main(int argc, char *argv[]) {
  GetOpt_pp args(argc, argv);
  args.exceptions_all();

  try {
    if (args >> OptionPresent('h', "help")) {
      show_usage();
      return 0;
    }
    if (args >> OptionPresent('v', "verbose")) {
      LOG.subscribeCout("info");
      LOG.subscribeCout("warning");
    }
    LOG.subscribeFile("log.txt", "info");
    LOG.subscribeFile("log.txt", "warning");

    string filename;
    args >> Option('f', "configfile", filename, "");
    if (filename.empty()) {
      cerr << "Configuration file name not specified. Type 'ltr_client -h' for usage." << endl;
      return 0;
    }
    args.end_of_options();

    Factory factory;
    RegisterAllTypes(&factory);

    LtrClient client;
    client.initFrom(filename);
    client.launch();

  } catch(TooManyOptionsEx) {
    cerr << "Unrecognized options specified. Type 'ltr_client -h' for usage." << endl;
  } catch(const GetOptEx& err) {
    cerr << "Invalid parameters. Type 'ltr_client -h' for usage." << endl;
  } catch(const logic_error& err) {
    cerr << "Failed: " << err.what() << endl;
  } catch(...) {
    cerr << "Caught exception" << endl;
  }
  return 0;
}
