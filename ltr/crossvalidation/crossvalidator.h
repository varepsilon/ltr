// Copyright 2012 Yandex
#ifndef LTR_CROSSVALIDATION_CROSSVALIDATOR_H_
#define LTR_CROSSVALIDATION_CROSSVALIDATOR_H_

#include <logog/logog.h>

#include <algorithm>
#include <list>
#include <string>
#include <vector>

#include "boost/multi_array.hpp"
#include "boost/range.hpp"
#include "boost/tuple/tuple.hpp"

#include "ltr/crossvalidation/splitter.h"
#include "ltr/data/data_set.h"
#include "ltr/learners/learner.h"
#include "ltr/measures/measure.h"
#include "ltr/utility/multitable.h"


using std::cerr;
using std::cout;
using std::list;
using std::reverse;
using std::sort;
using std::string;
using std::vector;

using ltr::utility::MultiTable;

namespace ltr {
namespace cv {

template <typename ObjectType, typename ScorerType>
class CrossValidator {
 public:
  CrossValidator();
  void addDataSet(const typename DataSet<ObjectType>::Ptr& data_set);
  void addMeasure(const typename Measure<ObjectType>::Ptr& measure);
  void addLearner(const typename Learner<ObjectType>::Ptr& learner);
  void addSplitter(const typename Splitter<ObjectType>::Ptr& splitter);
  void launch();
  void reset();
  void toString();
 private:
  void setCrossValidationResultLabels();
  MultiTable<double, 4> cross_validation_results_;
  vector<typename ltr::DataSet<ObjectType>::Ptr> data_sets_;
  vector<typename ltr::Measure<ObjectType>::Ptr> measures_;
  vector<typename ltr::Learner<ObjectType>::Ptr> learners_;
  vector<typename ltr::cv::Splitter<ObjectType>::Ptr> splitters_;
};

template <typename ObjectType, typename ScorerType>
CrossValidator<ObjectType, ScorerType>::CrossValidator()
  : cross_validation_results_()
  , data_sets_()
  , measures_()
  , learners_()
  , splitters_() {}

template <typename ObjectType, typename ScorerType>
void CrossValidator<ObjectType, ScorerType>::
  addDataSet(const typename DataSet<ObjectType>::Ptr& data_set) {
  data_sets_.push_back(data_set);
}

template <typename ObjectType, typename ScorerType>
void CrossValidator<ObjectType, ScorerType>::
  addMeasure(const typename Measure<ObjectType>::Ptr& measure) {
  measures_.push_back(measure);
}

template <typename ObjectType, typename ScorerType>
void CrossValidator<ObjectType, ScorerType>::addLearner(
    const typename Learner<ObjectType>::Ptr& learner
    ) {
  learners_.push_back(learner);
}

template <typename ObjectType, typename ScorerType>
void CrossValidator<ObjectType, ScorerType>::
  addSplitter(const typename Splitter<ObjectType>::Ptr& splitter) {
  splitters_.push_back(splitter);
}

template <typename ObjectType, typenmae ScorerType>
void CrossValidator<objectType, ScorerType>::
  setCrossValidationResultLabels() {
  INFO("Starting to set crossvalidation results labels.");
  INFO("Starting to set measures");
  cross_validation_results_.setAxisLabel(0, "Measure");
  for (int measure_index = 0;
       measure_index < measures_.size();
       ++measure_index) {
    INFO("Setting %d tick label. Current measure is %s",
         measure_index,
         measures_[measure_index]->alias());
    cross_validation_results_.setTickLabel(0,
                                           measure_index,
                                           measures_[measure_index]->alias());
  }

  INFO("Starting to set dataset");
  cross_validation_results_.setAxisLabel(1, "DataSet");
  for (int dataset_index = 0;
       dataset_index < data_sets_.size();
       ++dataset_index) {
    INFO("Setting %d tick label", dataset_index);
    cross_validation_results_.setTickLabel(1, dataset_index, "dataset");
  }

  INFO("Starting to set Learners");
  cross_validation_results_.setAxisLabel(2, "Learner");
  for (int learner_index = 0;
       learner_index < learners_.size();
       ++learner_index) {
    INFO("Setting %d tick label", learner_index);
    cross_validation_results_.
        setTickLabel(2, learner_index, learners_[learner_index]->alias());
  }

  INFO("Starting to set splitters");
  cross_validation_results_.setAxisLabel(3, "Splitter");
  for (int splitter_index = 0;
       splitter_index < splitters_.size();
       ++splitter_index) {
    INFO("Setting %d tick label", splitter_index);
    cross_validation_results_.
        setTickLabel(3, splitter_index, splitters_[splitter_index]->alias());
  }
}


template <typename ObjectType, typename ScorerType>
void CrossValidator<ObjectType, ScorerType>::launch() {
  INFO("Launching crossvalidator");
  vector<size_t> multi_size;
  multi_size.push_back(measures_.size());
  multi_size.push_back(data_sets_.size());
  multi_size.push_back(learners_.size());
  multi_size.push_back(splitters_.size());
  cross_validation_results_.resize(multi_size);
  setCrossValidationResultLabels();

  for (MultiTable<double, 4>::Iterator it = cross_validation_results_.begin();
       it != cross_validation_results_.end();
       ++it) {
    size_t measure_index = it.getMultiIndex()[0];
    size_t dataset_index = it.getMultiIndex()[1];
    size_t learner_index = it.getMultiIndex()[2];
    size_t splitter_index = it.getMultiIndex()[3];
    for (size_t split_index = 0;
         split_index < (*splitters_[splitter_index]).
         splitCount(*data_sets_[dataset_index]);
         ++split_index) {
      SplittedDataSet<ObjectType> splitted_data(
            splitters_[splitter_index]->split(
              split_index, *data_sets_[dataset_index]));
      learners_[learner_index]->reset();
      learners_[learner_index]->learn(splitted_data.train_set);

      Scorer::Ptr current_scorer = (
            learners_[learner_index])->make();
      current_scorer->predict(splitted_data.test_set);

      *it +=
          measures_[measure_index]->average(splitted_data.test_set) /
          (*splitters_[splitter_index]).splitCount(
            *data_sets_[dataset_index]);
    }
  }
}

template <typename ObjectType, typename ScorerType>
void CrossValidator<ObjectType, ScorerType>::reset() {
  cross_validation_results_.clear();
  measures_.clear();
  data_sets_.clear();
  learners_.clear();
  splitters_.clear();
}

template <typename ObjectType, typename ScorerType>
void CrossValidator<ObjectType, ScorerType>::toString() {
  return cross_validation_results_.toString();
}
}
}

#endif  // LTR_CROSSVALIDATION_CROSSVALIDATOR_H_
