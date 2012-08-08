// Copyright 2012 Yandex

#ifndef LTR_CROSSVALIDATION_CROSSVALIDATOR_H_
#define LTR_CROSSVALIDATION_CROSSVALIDATOR_H_

#include <algorithm>
#include <list>
#include <string>
#include <vector>

#include "boost/multi_array.hpp"
#include "boost/range.hpp"
#include "boost/tuple/tuple.hpp"

#include "logog/logog.h"

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
template <typename TElement>
class CrossValidator {
 public:
  CrossValidator();
  void addDataSet(typename DataSet<TElement>::Ptr data_set);
  void addMeasure(typename Measure<TElement>::Ptr measure);
  void addLearner(typename Learner<TElement>::Ptr learner);
  void addSplitter(typename Splitter<TElement>::Ptr splitter);
  void launch();
  void reset();
  string toString();
 private:
  void setCrossValidationResultLabels();
  MultiTable<double, 4> cross_validation_results_;
  vector<typename ltr::DataSet<TElement>::Ptr> data_sets_;
  vector<typename ltr::Measure<TElement>::Ptr> measures_;
  vector<typename ltr::Learner<TElement>::Ptr> learners_;
  vector<typename ltr::cv::Splitter<TElement>::Ptr> splitters_;
};

template <typename TElement>
CrossValidator<TElement>::CrossValidator()
  : cross_validation_results_()
  , data_sets_()
  , measures_()
  , learners_()
  , splitters_() {}

template <typename TElement>
void CrossValidator<TElement>::
  addDataSet(typename DataSet<TElement>::Ptr data_set) {
  data_sets_.push_back(data_set);
}

template <typename TElement>
void CrossValidator<TElement>::
  addMeasure(typename Measure<TElement>::Ptr measure) {
  measures_.push_back(measure);
}

template <typename TElement>
void CrossValidator<TElement>::addLearner(
  typename Learner<TElement>::Ptr learner
    ) {
  learners_.push_back(learner);
}

template <typename TElement>
void CrossValidator<TElement>::
  addSplitter(typename Splitter<TElement>::Ptr splitter) {
  splitters_.push_back(splitter);
}

template <typename TElement>
void CrossValidator<TElement>::
  setCrossValidationResultLabels() {
  INFO("Starting to set crossvalidation results labels.");
  INFO("Starting to set measures");
  cross_validation_results_.setAxisLabel(0, "Measure");
  for (int measure_index = 0;
       measure_index < (int)measures_.size();
       ++measure_index) {
    INFO("Setting %d tick label. Current measure is %s",
         measure_index,
         measures_[measure_index]->alias().c_str());
    cross_validation_results_.setTickLabel(0,
                                           measure_index,
                                           measures_[measure_index]->alias());
  }

  INFO("Starting to set dataset");
  cross_validation_results_.setAxisLabel(1, "DataSet");
  for (int dataset_index = 0;
       dataset_index < (int)data_sets_.size();
       ++dataset_index) {
    INFO("Setting %d tick label", dataset_index);
    cross_validation_results_.setTickLabel(1, dataset_index, "dataset");
  }

  INFO("Starting to set Learners");
  cross_validation_results_.setAxisLabel(2, "Learner");
  for (int learner_index = 0;
       learner_index < (int)learners_.size();
       ++learner_index) {
    INFO("Setting %d tick label", learner_index);
    cross_validation_results_.
        setTickLabel(2, learner_index, learners_[learner_index]->alias());
  }

  INFO("Starting to set splitters");
  cross_validation_results_.setAxisLabel(3, "Splitter");
  for (int splitter_index = 0;
       splitter_index < (int)splitters_.size();
       ++splitter_index) {
//    INFO("Setting %d tick label", splitter_index);
    cross_validation_results_.
        setTickLabel(3, splitter_index, splitters_[splitter_index]->alias());
  }
}

template <typename TElement>
void CrossValidator<TElement>::launch() {
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
    for (int split_index = 0;
         split_index < (*splitters_[splitter_index]).
         splitCount(*data_sets_[dataset_index]);
         ++split_index) {
      SplittedDataSet<TElement> splitted_data(
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

template <typename TElement>
void CrossValidator<TElement>::reset() {
  cross_validation_results_.clear();
  measures_.clear();
  data_sets_.clear();
  learners_.clear();
  splitters_.clear();
}

template <typename TElement>
string CrossValidator<TElement>::toString() {
  return cross_validation_results_.toString();
}
};
};

#endif  // LTR_CROSSVALIDATION_CROSSVALIDATOR_H_
