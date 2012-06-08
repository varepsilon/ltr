// Copyright 2011 Yandex

#include "ltr_client/ltr_client.h"

#include <iostream>
#include <vector>

using std::cout;

#include "boost/algorithm/string.hpp"
#include "ltr_client/configurator.h"
#include "ltr_client/factory.h"
#include "ltr/learners/learner.h"
#include "ltr/data/utility/io_utility.h"

// For factory purposes
#include "ltr/learners/best_feature_learner/best_feature_learner.h"
#include "ltr/measures/ndcg.h"

#include "contrib/logog/include/logog.hpp"

class LtrClientPrivate {
 public:
  template <class TElement>
  void executeTrain(ltr::Parameterized* parameterized,
                const TTrainInfo &train_info);

  Configurator configurator;
  TXmlTokenSpecList getLoadQueue() const;
};

static bool BuildObjectCreationChain(const TXmlTokenSpec *spec,
                           TXmlTokenSpecList *queue,
                           TXmlTokenSpecList *circularity_check_queue) {
  if (std::find(queue->begin(), queue->end(), spec) != queue->end())
    return true;

  // check for circular dependencies
  TXmlTokenSpecList::const_iterator begin_it =
      std::find(circularity_check_queue->begin(),
                circularity_check_queue->end(),
                spec);

  if (begin_it != circularity_check_queue->end())
    throw std::logic_error("Circular dependency detected!");
  circularity_check_queue->push_back(spec);

  // add dependencies
  for (TXmlTokenSpecList::const_iterator it = spec->dependencySpecs().begin();
      it != spec->dependencySpecs().end();
      ++it) {
    const TXmlTokenSpec *dep_spec = *it;
    if (!BuildObjectCreationChain(dep_spec, queue, circularity_check_queue))
      throw std::logic_error("Can not resolve dependencies!");
  }

  // add self
  queue->push_back(spec);
  return true;
}

static TXmlTokenSpecList values(const Configurator::TXmlTokenSpecs &cont) {
  TXmlTokenSpecList values;
  for (Configurator::TXmlTokenSpecs::const_iterator it = cont.begin();
      it != cont.end();
      ++it) {
    const TXmlTokenSpec &spec = it->second;
    values.push_back(&spec);
  }
  return values;
}

TXmlTokenSpecList LtrClientPrivate::getLoadQueue() const {
  const TXmlTokenSpecList &all_specs = values(configurator.xmlTokenSpecs());

  TXmlTokenSpecList result;
  for (TXmlTokenSpecList::const_iterator it = all_specs.begin();
      it != all_specs.end();
      ++it) {
    const TXmlTokenSpec *spec = *it;
    TXmlTokenSpecList circularity_check_queue;
    BuildObjectCreationChain(spec, &result, &circularity_check_queue);
  }
  return result;
}


LtrClient::LtrClient(): d(new LtrClientPrivate) {
}
LtrClient::~LtrClient() {
  delete d;
}

static ltr::ParametersContainer Create(
    const ltr::ParametersContainer &src_parameters,
    const Configurator::TXmlTokenSpecs &all_specs);

static ltr::Parameterized *Create(const std::string &name,
                               const Configurator::TXmlTokenSpecs &all_specs) {
  Configurator::TXmlTokenSpecs::const_iterator it = all_specs.find(name);
  assert(it != all_specs.end());
  const TXmlTokenSpec *spec = &it->second;
  const ltr::ParametersContainer &parameters =
      Create(spec->getParameters(), all_specs);
  return Factory::instance()->Create(spec->getType(), parameters);
}
static ltr::ParametersContainer Create(
    const ltr::ParametersContainer &src_parameters,
    const Configurator::TXmlTokenSpecs &all_specs) {

  ltr::ParametersContainer result;
  for (ltr::ParametersContainer::StringAnyHash::const_iterator it =
       src_parameters.begin();
      it != src_parameters.end();
      ++it) {
    const boost::any &parameter = it->second;
    const std::string &name = it->first;

    if (const TXmlTokenDependency *dependency =
       boost::any_cast<TXmlTokenDependency>(&parameter)) {
      typedef std::vector<std::string> TStringVector;
      TStringVector strings;
      boost::split(strings, dependency->parameter_name,
                   boost::is_any_of("\t "));
      assert(strings.size());
      if (strings.size() == 1) {
        result.AddNew(name, Create(strings[0], all_specs));
      } else {
        std::vector<boost::any> list;
        for (TStringVector::const_iterator str_it = strings.begin();
            str_it != strings.end();
            ++str_it) {
          list.push_back(Create(*str_it, all_specs));
        }
        result.AddNew(name, list);
      }
    } else if (const ltr::ParametersContainer *cont =
              boost::any_cast<ltr::ParametersContainer>(&parameter)) {
      result.AddNew(name, Create(*cont, all_specs));
    } else {
      result.AddNew(name, parameter);
    }
  }
  return result;
}

void LtrClient::initFrom(const std::string &file_name) {
  d->configurator.loadConfig(file_name);
  d->getLoadQueue();  // check absence of circularity dependencies
  /*const Configurator::TXmlTokenSpecs queue = d->getLoadQueue();
  for (Configurator::TXmlTokenSpecs::const_iterator it = queue.begin();
      it != queue.end();
      ++it) {
    const TXmlTokenSpec *spec = *it;
    std::cout << "initFrom  " << ToString(*spec) << std::endl << std::endl;
  }*/
}

template <class TElement>
void LtrClientPrivate::executeTrain(ltr::Parameterized *parameterized,
                                const TTrainInfo &train_info) {
  ltr::BaseLearner<TElement> *learner =                           //NOLINT
      dynamic_cast<ltr::BaseLearner<TElement> *>(parameterized);  //NOLINT
  assert(learner);

  const TXmlTokenSpec &learner_info = configurator.findLearner(
                                                           train_info.learner);

  const TDataInfo &data_info = configurator.findData(train_info.data);
  if (learner_info.getApproach() != data_info.approach)
    throw std::logic_error("Approach of learner and data does not coincide!");

  const ltr::DataSet<TElement> &data_set =
                    ltr::io_utility::loadDataSet<TElement>(data_info.file_name,
                                                            data_info.format);

  std::cout << "\n\n\nvoid doLaunch\n";
  learner->learn(data_set);

  std::cout << "\n\nTrain " << data_info.file_name << " finished. Report:"
                        << learner->report() << std::endl;

  for (boost::unordered_set<std::string>::const_iterator predict_it =
      train_info.predicts.begin();
      predict_it != train_info.predicts.end();
      ++predict_it) {
    const std::string &predict = *predict_it;
    if (configurator.dataInfos().find(predict) ==
       configurator.dataInfos().end()) {
      std::cout << "Can't predict. Unknown data " << predict << std::endl;
      return;
    }
    const std::string &predict_file_path = configurator.rootPath() +
                                   learner_info.getName() + "."
                                   + predict + ".predicts";

    ltr::Scorer::Ptr scorer = learner->makeScorerPtr();
    ltr::io_utility::savePredictions(data_set, scorer, predict_file_path);
    std::cout << "\nsaved predictions for '" << predict
                                    << "' into " << predict_file_path
                                    << std::endl;

    if (train_info.gen_cpp) {
        std::string cpp_file_path = configurator.rootPath() +
                                    learner_info.getName() +
                                    ".cpp";
        std::ofstream fout(cpp_file_path.c_str());
        fout << scorer->generateCppCode(learner_info.getName());
        fout.close();
        std::cout  << "cpp code saved into " << cpp_file_path << std::endl;
    }
  }
}

void LtrClient::launch() {
  for (Configurator::TTrainInfos::const_iterator it =
       d->configurator.trainInfos().begin();
      it != d->configurator.trainInfos().end();
      ++it) {
    const TTrainInfo &train_info = it->second;
    const TXmlTokenSpec &learner_info = d->configurator.findLearner(
                                          train_info.learner);

    const ltr::ParametersContainer &parameters = Create(
                                               learner_info.getParameters(),
                                               d->configurator.xmlTokenSpecs());
    std::cout << "\nvoid LtrClient::launch()\n  parameters =" <<
                 parameters.toString() << "\n";
    ltr::Parameterized*
        parameterized = Factory::instance()->Create(learner_info.getType() +
                                                    learner_info.getApproach(),
                                                    parameters);

    if (learner_info.getApproach() == "listwise") {
      d->executeTrain<ltr::ObjectList>(parameterized, train_info);
    } else if (learner_info.getApproach() == "pairwise") {
      d->executeTrain<ltr::ObjectPair>(parameterized, train_info);
    } else {
      assert(false && "Not implemented yet");
    }
  }
}

// #define LOGOG_GROUP NULL

// ===========================================================================

int main(int argc, char* argv[]) {
  LOGOG_INITIALIZE();
//  logog::Cout out;
//  logog::LogFile outFile("ltr_client.log");
//  logog::GetDefaultFormatter().SetShowTimeOfDay(true);

  if (argc < 2) {
      ERR("config file  missing");
      return 1;
  }
  Factory factory;

  factory.registerType<ltr::BestFeatureLearner<ltr::ObjectList> >
      ("BEST_FEATURElistwise");
  factory.registerType<ltr::NDCG>("NDCG");
  cout << "!!!!!!!!";
  LtrClient client;
  try {
      client.initFrom(argv[1]);
      client.launch();
  } catch(const std::logic_error &err) {
      ERR("Failed: %s", err.what());
  } catch(...) {
      ERR("Caught exception");
  }

  LOGOG_SHUTDOWN();
  return 0;
}
