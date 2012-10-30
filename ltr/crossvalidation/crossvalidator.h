// Copyright 2012 Yandex
#ifndef LTR_CROSSVALIDATION_CROSSVALIDATOR_H_
#define LTR_CROSSVALIDATION_CROSSVALIDATOR_H_

#include <rlog/rlog.h>

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
  GET_SET_VECTOR(typename ltr::DataSet<ObjectType>, data_set);
  GET_SET_VECTOR_OF_PTR(typename ltr::Measure<ObjectType>, measure);
  GET_SET_VECTOR_OF_PTR(typename ltr::Learner<ObjectType>, learner);
  GET_SET_VECTOR_OF_PTR(typename ltr::cv::Splitter<ObjectType>, splitter);
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
  vector<typename ltr::DataSet<ObjectType> > data_set_;
  vector<typename ltr::Measure<ObjectType>::Ptr> measure_;
  vector<typename ltr::Learner<ObjectType>::Ptr> learner_;
  vector<typename ltr::cv::Splitter<ObjectType>::Ptr> splitter_;
};

template <typename ObjectType>
CrossValidator<ObjectType>::CrossValidator()
  : cross_validation_results_()
  , data_set_()
  , measure_()
  , learner_()
  , splitter_() {}

template <typename ObjectType>
void CrossValidator<ObjectType>::
  setCrossValidationResultLabels() {
  rDebug("Starting to set crossvalidation results labels.");
  rDebug("Starting to set measures");
  cross_validation_results_.setAxisLabel(0, "Measure");
  for (int measure_index = 0;
       measure_index < measure_.size();
       ++measure_index) {
    rDebug("Setting %d tick label. Current measure is %s",
         measure_index,
         measure_[measure_index]->alias().c_str());
    cross_validation_results_.setTickLabel(0,
                                           measure_index,
                                           measure_[measure_index]->alias());
  }

  rDebug("Starting to set dataset");
  cross_validation_results_.setAxisLabel(1, "DataSet");
  for (int dataset_index = 0;
       dataset_index < data_set_.size();
       ++dataset_index) {
    rDebug("Setting %d tick label", dataset_index);
    cross_validation_results_.
        setTickLabel(1,
                     dataset_index,
                     data_set_[dataset_index].alias());
  }

  rDebug("Starting to set Learners");
  cross_validation_results_.setAxisLabel(2, "Learner");
  for (int learner_index = 0;
       learner_index < learner_.size();
       ++learner_index) {
    rDebug("Setting %d tick label", learner_index);
    cross_validation_results_.
        setTickLabel(2, learner_index, learner_[learner_index]->alias());
  }

  rDebug("Starting to set splitters");
  cross_validation_results_.setAxisLabel(3, "Splitter");
  for (int splitter_index = 0;
       splitter_index < splitter_.size();
       ++splitter_index) {
    cross_validation_results_.
        setTickLabel(3, splitter_index, splitter_[splitter_index]->alias());
  }
}


template <typename ObjectType>
void CrossValidator<ObjectType>::launch() {
  rDebug("Launching crossvalidator");
  vector<size_t> multi_size;
  multi_size.push_back(measure_.size());
  multi_size.push_back(data_set_.size());
  multi_size.push_back(learner_.size());
  multi_size.push_back(splitter_.size());
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
         split_index < (*splitter_[splitter_index]).
         splitCount(data_set_[dataset_index]);
         ++split_index) {
      SplittedDataSet<ObjectType> splitted_data(
            splitter_[splitter_index]->split(
              split_index, data_set_[dataset_index]));

      learner_[learner_index]->reset();
      learner_[learner_index]->learn(splitted_data.train_set);

      Scorer::Ptr current_scorer = (
            learner_[learner_index])->make();
      current_scorer->predict(splitted_data.test_set);

      *it +=
          measure_[measure_index]->average(splitted_data.test_set) /
          (*splitter_[splitter_index]).splitCount(
            data_set_[dataset_index]);
    }
  }
}

template <typename ObjectType>
void CrossValidator<ObjectType>::reset() {
  cross_validation_results_.clear();
  measure_.clear();
  data_set_.clear();
  learner_.clear();
  splitter_.clear();
}

template <typename ObjectType>
string CrossValidator<ObjectType>::toString() {
  return cross_validation_results_.toString();
}
}
}

#endif  // LTR_CROSSVALIDATION_CROSSVALIDATOR_H_
