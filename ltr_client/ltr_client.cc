// Copyright 2012 Yandex

#include <stdexcept>
#include <string>
#include <vector>
#include <ctime>

#include "rlog/rlog_default.h"

#include "ltr_client/ltr_client.h"
#include "ltr_client/factory.h"
#include "ltr_client/config_constants.h"
#include "ltr/crossvalidation/splitter.h"
#include "ltr/crossvalidation/crossvalidator.h"
#include "ltr/data/utility/io_utility.h"
#include "ltr/learners/learner.h"
#include "ltr/measures/measure.h"
#include "ltr/metrics/metric.h"
#include "ltr/utility/boost/path.h"
#include "ltr/utility/boost/string_utils.h"
#include "ltr/utility/html.h"
#include "ltr/utility/neighbor_weighter.h"

using std::find;
using std::logic_error;
using std::exception;
using std::string;
using std::vector;
using std::set;
using std::endl;
using std::ofstream;

using ltr::Parameterized;
using ltr::ParametersContainer;
using ltr::Learner;
using ltr::DataSet;
using ltr::Scorer;
using ltr::ObjectList;
using ltr::ObjectPair;
using ltr::Measure;
using ltr::cv::Splitter;
using ltr::cv::CrossValidator;

using ltr::io_utility::loadDataSet;
using ltr::utility::AppendTrailingPathSeparator;
using ltr::utility::FileLink;
using ltr::utility::split;


LtrClient::LtrClient(): configuration_(new Configuration) {}

LtrClient::~LtrClient() {}

void LtrClient::initFrom(const string& file_name) {
  addToReport("<h3>Experiment with LTR Client</h3>");
  time_t current_time(time(NULL));
  addToReport("<p>Started on " +
              static_cast<string>(ctime(&current_time)) + ".</p>");
  configuration_ = config_parser_.parse(file_name);
  factory_.init(configuration_);
  factory_.checkCircularDependencyAbsence();
}

void LtrClient::addToReport(const string& text) {
  report_body_ += "\n" + text;
}

void LtrClient::saveReport(const string& file_name) const {
  ofstream file_out(file_name.c_str());
  file_out << "<!DOCTYPE html>\n<html>\n<head>\n<title>"
           << "Experiment with LTR Client"
           << "</title>\n</head>\n<body>"
           << report_body_
           << "</body>\n</html>";
  file_out.close();
  rInfo("Report saved into %s\n", file_name.c_str());
}

template <class TElement>
void LtrClient::launchTrainImpl(typename Learner<TElement>::Ptr learner,
                                const TrainLaunchInfo::Ptr train_info) {
  const ObjectInfo::Ptr learner_info(
      configuration_->object_infos.safeAt(train_info->learner));

  const DataInfo::Ptr data_info(
      configuration_->data_infos.safeAt(train_info->data));

  if (learner_info->approach != data_info->approach) {
    throw logic_error("approaches of learner and data do not coincide");
  }

  const DataSet<TElement>& data_set =
    loadDataSet<TElement>(data_info->file, data_info->format);

  rInfo("Train started.");
  learner->learn(data_set);

  rInfo("Train %s finished. Learner's report: '%s'.", data_info->file.c_str(),
       learner->report().c_str());

  Scorer::Ptr scorer = learner->make();

  rInfo("Predicting started.");
  for (std::set<string>::const_iterator predict_it =
       train_info->predicts.begin();
       predict_it != train_info->predicts.end();
       ++predict_it) {
    const string& predict_data_name = *predict_it;
    if (!configuration_->data_infos.contains(predict_data_name)) {
      rWarning("Can't predict. Unknown data %s\n", predict_data_name.c_str());
      continue;
    }
    DataInfo::Ptr predict_data_info =
        configuration_->data_infos.safeAt(*predict_it);
    const DataSet<TElement>& predict_data_set =
        loadDataSet<TElement>(predict_data_info->file,
                              predict_data_info->format);
    const string& predict_file_path =
      AppendTrailingPathSeparator(configuration_->root_path) +
      learner_info->name + "." + predict_data_name + ".predicts";

    ltr::io_utility::savePredictions(predict_data_set,
                                     scorer,
                                     predict_file_path);

    rInfo("Predictions for '%s' saved into %s", predict_data_name.c_str(),
          predict_file_path.c_str());
    addToReport("<p>\n\tPredictions for <i>" + predict_data_name +
                "</i> saved to " + FileLink(predict_file_path) + ".\n</p>");
  }
  if (train_info->gen_cpp) {
    string cpp_file_path =
      AppendTrailingPathSeparator(configuration_->root_path) +
      learner_info->name + ".cpp";
    ofstream file_out(cpp_file_path.c_str());
    file_out << scorer->generateCppCode(learner_info->name);
    file_out.close();
    rInfo("Cpp code saved into %s", cpp_file_path.c_str());
    addToReport("<p>\n\tCpp code for <i>" + learner_info->name +
                "</i> saved to " + FileLink(cpp_file_path) + ".\n</p>");
  }
}

template <class TElement>
void LtrClient::launchCrossvalidationImpl(
  const CrossvalidationLaunchInfo::Ptr crossvalidation_info) {
    typedef set<string>::const_iterator TSetIterator;
    CrossValidator<TElement> cross_validator;

    for (TSetIterator learners_alias = crossvalidation_info->learners.begin();
        learners_alias != crossvalidation_info->learners.end();
        ++learners_alias) {
      cross_validator.add_learner(
          factory_.CreateObject<typename Learner<TElement>::Ptr>(
              *learners_alias,
              LEARNER));
    }

    for (TSetIterator measures_alias = crossvalidation_info->measures.begin();
         measures_alias != crossvalidation_info->measures.end();
         ++measures_alias) {
      cross_validator.add_measure(
          factory_.CreateObject<typename Measure<TElement>::Ptr>(
              *measures_alias,
              MEASURE));
    }

    cross_validator.add_splitter(
        factory_.CreateObject<typename Splitter<TElement>::Ptr>(
            crossvalidation_info->splitter,
            SPLITTER));

    for (TSetIterator datas_alias = crossvalidation_info->datas.begin();
         datas_alias != crossvalidation_info->datas.end();
         ++datas_alias) {
      const DataInfo::Ptr data_info =
          configuration_->data_infos.safeAt(*datas_alias);
      DataSet<TElement> data_set =
          loadDataSet<TElement>(data_info->file, data_info->format);
      cross_validator.add_data_set(data_set);
    }

    cross_validator.launch();
    rInfo("%s", cross_validator.toString().c_str());
    addToReport("<p><b>Crossvalidation results:</b></p>\n" +
        cross_validator.toHTML());
}

void LtrClient::launchTrain() {
  for (Configuration::TrainInfos::const_iterator iterator =
          configuration_->train_infos.begin();
       iterator != configuration_->train_infos.end();
       ++iterator) {
    const TrainLaunchInfo::Ptr train_info = iterator->second;
    const ObjectInfo::Ptr learner_info =
      configuration_->object_infos.safeAt(train_info->learner);

    if (learner_info->approach == "listwise") {
      launchTrainImpl<ObjectList>(
        factory_.CreateObject<Learner<ObjectList>::Ptr>(train_info->learner,
                                                        LEARNER),
        train_info);
    } else if (learner_info->approach == "pairwise") {
      launchTrainImpl<ObjectPair>(
        factory_.CreateObject<Learner<ObjectPair>::Ptr>(train_info->learner,
                                                        LEARNER),
        train_info);
    } else if (learner_info->approach == "pointwise") {
      launchTrainImpl<Object>(
        factory_.CreateObject<Learner<Object>::Ptr>(train_info->learner,
                                                    LEARNER),
        train_info);
    } else {
      throw logic_error("unknown learner's approach");
    }
  }
}

void LtrClient::launchCrossvalidation() {
  for (Configuration::CrossvalidationInfos::const_iterator iterator =
       configuration_->crossvalidation_infos.begin();
       iterator != configuration_->crossvalidation_infos.end();
       ++iterator) {
    const CrossvalidationLaunchInfo::Ptr crossvalidation_info =
      iterator->second;
    const ObjectInfo::Ptr learner_info =
      configuration_->object_infos.safeAt(
        *(crossvalidation_info->learners.begin()));

    if (learner_info->approach == "listwise") {
      launchCrossvalidationImpl<ObjectList>(crossvalidation_info);
    } else if (learner_info->approach == "pairwise") {
      launchCrossvalidationImpl<ObjectPair>(crossvalidation_info);
    } else if (learner_info->approach == "pointwise") {
      launchCrossvalidationImpl<Object>(crossvalidation_info);
    } else {
      throw logic_error("unknown learner's approach");
    }
  }
}

void LtrClient::launch() {
  rInfo("Experiment started.");
  launchTrain();
  launchCrossvalidation();

  time_t current_time(time(NULL));
  addToReport("<p>Finished on " +
              static_cast<string>(ctime(&current_time)) + ".</p>");
  addToReport("<hr/>&copy; Yandex, 2011");
  string report_path = AppendTrailingPathSeparator(configuration_->root_path) +
      "report.html";
  saveReport(report_path);
}
