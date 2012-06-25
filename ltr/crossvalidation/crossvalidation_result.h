// Copyright 2012 Yandex
#ifndef LTR_CROSSVALIDATION_CROSSVALIDATION_RESULT_H_
#define LTR_CROSSVALIDATION_CROSSVALIDATION_RESULT_H_
#include <vector>
#include "ltr/measures/measure.h"
#include "ltr/learners/learner.h"
#include "ltr/crossvalidation/splitter.h"
#include "ltr/data/data_set.h"
#include "ltr/scorers/utility/scorer_utility.h"

using std::vector;
using ltr::utility::MarkDataSet;
using std::cout;

namespace ltr {
namespace cv {
template <typename ObjectType, typename ScorerType>
class CrossValidator {
  vector<vector<vector<vector<vector<double> > > > > crossValidationResults_;
  vector<typename ltr::DataSet<ObjectType>::Ptr > dataSets_;
  vector<typename ltr::Measure<ObjectType>::Ptr> measures_;
  vector<typename ltr::Learner<ObjectType, ScorerType>::Ptr> learners_;
  vector<typename ltr::cv::Splitter<ObjectType>::Ptr > splitters_;
 public:
  CrossValidator()
    : crossValidationResults_()
    , dataSets_()
    , measures_()
    , learners_()
    , splitters_() {}

  void addDataSet(const typename DataSet<ObjectType>::Ptr& dataSet) {
    dataSets_.push_back(dataSet);
  }

  void addMeasure(const typename Measure<ObjectType>::Ptr& measure) {
    measures_.push_back(measure);
  }

  void addLearner(
      const typename Learner<ObjectType, ScorerType>::Ptr& learner
      ) {
    learners_.push_back(learner);
  }

  void addSplitter(const typename Splitter<ObjectType>::Ptr& splitter) {
    splitters_.push_back(splitter);
  }

  void launch() {
    crossValidationResults_.clear();
    crossValidationResults_.resize(dataSets_.size());
    for (size_t datasetIndex = 0;
         datasetIndex < dataSets_.size();
         ++datasetIndex) {
      crossValidationResults_[datasetIndex].resize(measures_.size());
      for (size_t measureIndex = 0;
           measureIndex < measures_.size();
           ++measureIndex) {
        crossValidationResults_[datasetIndex]
            [measureIndex].resize(learners_.size());
        for (size_t learnerIndex = 0;
             learnerIndex < learners_.size();
             ++learnerIndex) {
          crossValidationResults_
              [datasetIndex][measureIndex]
              [learnerIndex].resize(splitters_.size());
          for (size_t splitterIndex = 0;
               splitterIndex < splitters_.size();
               ++splitterIndex) {
            for (size_t splitIndex = 0;
                 splitIndex < (*splitters_[splitterIndex]).
                 splitCount(*dataSets_[datasetIndex]);
                 ++splitIndex) {
              crossValidationResults_
                  [datasetIndex][measureIndex][learnerIndex][splitterIndex].
                  resize((*splitters_[splitterIndex]).splitCount(
                           *dataSets_[datasetIndex]));
              SplittedDataSet<ObjectType> splittedData(
                    splitters_[splitterIndex]->split(
                      splitIndex, *dataSets_[datasetIndex]));
              learners_[learnerIndex]->reset();
              learners_[learnerIndex]->learn(splittedData.train_set);

              Scorer::Ptr currentScorer = (
                    learners_[learnerIndex])->makeScorerPtr();
              MarkDataSet(splittedData.test_set, *currentScorer);

              crossValidationResults_
                  [datasetIndex][measureIndex][learnerIndex]
                  [splitterIndex][splitIndex] =
                  measures_[measureIndex]->average(splittedData.test_set);
              cout << measures_[measureIndex]->average(splittedData.test_set);
            }
          }
        }
      }
    }
  }

  void clearResults() {
    crossValidationResults_.clear();
  }

  void clearCrossValidator() {
    crossValidationResults_.clear();
    measures_.clear();
    dataSets_.clear();
    learners_.clear();
    splitters_.clear();
  }

  void printResults() {
    for (size_t dataSetIndex = 0;
         dataSetIndex < crossValidationResults_.size();
         ++dataSetIndex) {
      for (size_t measureIndex = 0;
           measureIndex < crossValidationResults_[dataSetIndex].size();
           ++measureIndex) {
        for (size_t learnerIndex = 0;
             learnerIndex < crossValidationResults_[dataSetIndex]
             [measureIndex].size();
             ++learnerIndex) {
          for (size_t splitterIndex = 0;
               splitterIndex < crossValidationResults_
               [dataSetIndex][measureIndex][learnerIndex].size();
               ++splitterIndex) {
            for (size_t splitIndex = 0;
                 splitIndex < crossValidationResults_
                 [dataSetIndex][measureIndex][learnerIndex]
                 [splitterIndex].size();
                 ++splitIndex) {
              cout << crossValidationResults_
                      [dataSetIndex][measureIndex][learnerIndex]
                      [splitterIndex][splitIndex] << "\n";
            }
          }
        }
      }
    }
  }
};
}
}

#endif  // LTR_CROSSVALIDATION_CROSSVALIDATION_RESULT_H_
