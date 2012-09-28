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


using std::list;
using std::reverse;
using std::sort;
using std::string;
using std::vector;

using ltr::utility::MultiTable;

namespace ltr {
namespace cv {

/**
 * Performs crossvalidation and forms a printable set
 * of tables called multitable.
 *
 * Info about cross-validation can be found here:
 * http://en.wikipedia.org/wiki/Cross-validation_(statistics)
 *
 * Multitable in CrossValidator contains 4 dimentions:
 *  -learners
 *  -splitters
 *  -measures
 *  -datasets
 *
 * To perform crossvalidation add to the crossvalidator datasets,
 * measures, splitters, learners. Every entity must be contained in
 * crossvalidtor at least once. After the initialization is done,
 * perform "launch()" command.
 *
 * Crossvalidator is printable
 */
template <typename ObjectType>
class CrossValidator {
 public:
  /**
   * Basic CrossValidator constructor
   */
  CrossValidator();
  /**
   * Adds a dataset to the CrossValidator instance
   * @param data_set - data set to be inserted into crossvalidator
   */
  void addDataSet(const DataSet<ObjectType>& data_set);
  /**
   * Adds a measure to the CrossValidator instance
   * @param measure - measure to be inserted into crossvalidator
   */
  void addMeasure(const typename Measure<ObjectType>::Ptr& measure);
  /**
   * Adds a learner to the CrossValidator instance
   * @param learner - data set to be inserted into crossvalidator
   */
  void addLearner(const typename Learner<ObjectType>::Ptr& learner);
  /**
   * Adds a splitter to the CrossValidator instance
   * @param splitter - data set to be inserted into crossvalidator
   */
  void addSplitter(const typename Splitter<ObjectType>::Ptr& splitter);
  /**
   * Launches crossvalidation.
   */
  void launch();
  /**
   * Resets crossvalidator: empties the containsers with datasets,
   * measures, splitters, learners.
   */
  void reset();
  /**
   * Converts a crossvalidation results to string.
   * @returns string with crossvalidation results.
   */
  string toString();
 private:
  /**
   * Sets the labels of the multitable instance according to the
   * information in the alias.
   */
  void setCrossValidationResultLabels();
  MultiTable<double, 4> cross_validation_results_;
  vector<typename ltr::DataSet<ObjectType> > data_sets_;
  vector<typename ltr::Measure<ObjectType>::Ptr> measures_;
  vector<typename ltr::Learner<ObjectType>::Ptr> learners_;
  vector<typename ltr::cv::Splitter<ObjectType>::Ptr> splitters_;
};

template <typename ObjectType>
CrossValidator<ObjectType>::CrossValidator()
  : cross_validation_results_()
  , data_sets_()
  , measures_()
  , learners_()
  , splitters_() {}

template <typename ObjectType>
void CrossValidator<ObjectType>::
  addDataSet(const DataSet<ObjectType>& data_set) {
  data_sets_.push_back(data_set);
}

template <typename ObjectType>
void CrossValidator<ObjectType>::
  addMeasure(const typename Measure<ObjectType>::Ptr& measure) {
  measures_.push_back(measure);
}

template <typename ObjectType>
void CrossValidator<ObjectType>::addLearner(
    const typename Learner<ObjectType>::Ptr& learner
    ) {
  learners_.push_back(learner);
}

template <typename ObjectType>
void CrossValidator<ObjectType>::
  addSplitter(const typename Splitter<ObjectType>::Ptr& splitter) {
  splitters_.push_back(splitter);
}

template <typename ObjectType>
void CrossValidator<ObjectType>::
  setCrossValidationResultLabels() {
  DBUG("Starting to set crossvalidation results labels.");
  DBUG("Starting to set measures");
  cross_validation_results_.setAxisLabel(0, "Measure");
  for (int measure_index = 0;
       measure_index < measures_.size();
       ++measure_index) {
    DBUG("Setting %d tick label. Current measure is %s",
         measure_index,
         measures_[measure_index]->alias().c_str());
    cross_validation_results_.setTickLabel(0,
                                           measure_index,
                                           measures_[measure_index]->alias());
  }

  DBUG("Starting to set dataset");
  cross_validation_results_.setAxisLabel(1, "DataSet");
  for (int dataset_index = 0;
       dataset_index < data_sets_.size();
       ++dataset_index) {
    DBUG("Setting %d tick label", dataset_index);
    cross_validation_results_.
        setTickLabel(1,
                     dataset_index,
                     data_sets_[dataset_index].alias());
  }

  DBUG("Starting to set Learners");
  cross_validation_results_.setAxisLabel(2, "Learner");
  for (int learner_index = 0;
       learner_index < learners_.size();
       ++learner_index) {
    DBUG("Setting %d tick label", learner_index);
    cross_validation_results_.
        setTickLabel(2, learner_index, learners_[learner_index]->alias());
  }

  DBUG("Starting to set splitters");
  cross_validation_results_.setAxisLabel(3, "Splitter");
  for (int splitter_index = 0;
       splitter_index < splitters_.size();
       ++splitter_index) {
    cross_validation_results_.
        setTickLabel(3, splitter_index, splitters_[splitter_index]->alias());
  }
}


template <typename ObjectType>
void CrossValidator<ObjectType>::launch() {
  DBUG("Launching crossvalidator");
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
         splitCount(data_sets_[dataset_index]);
         ++split_index) {
      SplittedDataSet<ObjectType> splitted_data(
            splitters_[splitter_index]->split(
              split_index, data_sets_[dataset_index]));

      learners_[learner_index]->reset();
      learners_[learner_index]->learn(splitted_data.train_set);

      Scorer::Ptr current_scorer = (
            learners_[learner_index])->make();
      current_scorer->predict(splitted_data.test_set);

      *it +=
          measures_[measure_index]->average(splitted_data.test_set) /
          (*splitters_[splitter_index]).splitCount(
            data_sets_[dataset_index]);
    }
  }
}

template <typename ObjectType>
void CrossValidator<ObjectType>::reset() {
  cross_validation_results_.clear();
  measures_.clear();
  data_sets_.clear();
  learners_.clear();
  splitters_.clear();
}

template <typename ObjectType>
string CrossValidator<ObjectType>::toString() {
  return cross_validation_results_.toString();
}
}
}

#endif  // LTR_CROSSVALIDATION_CROSSVALIDATOR_H_
