// Copyright 2012 Yandex
#ifndef LTR_CROSSVALIDATION_CROSSVALIDATION_RESULT_H_
#define LTR_CROSSVALIDATION_CROSSVALIDATION_RESULT_H_
#include <vector>
#include <algorithm>
#include <string>
#include "ltr/measures/measure.h"
#include "ltr/learners/learner.h"
#include "ltr/crossvalidation/splitter.h"
#include "ltr/data/data_set.h"
#include "ltr/scorers/utility/scorer_utility.h"

using std::vector;
using std::sort;
using std::reverse;
using ltr::utility::MarkDataSet;
using std::cout;
using std::string;

namespace ltr {
namespace cv {

struct SizeOverIndex {
  int indexVal;
  size_t sizeVal;
};

template <typename T>
int getSize(const T& value) {
  return 0;
}

template <typename T>
int getSize(const vector<T>& array) {
  return array.size();
}

template <typename T>
void getMultiSize(const T& value, vector<SizeOverIndex>* const result) {}

template <typename T>
void getMultiSize(const vector<T>& array, vector<SizeOverIndex>* const result) {
  SizeOverIndex newItem = {result->size(), array.size()};
  result->push_back(newItem);
  for (size_t i = 0; i < array.size(); ++i) {
    if (getSize(array[i]) != getSize(array[0])) {
      throw std::logic_error(
            "In getMultiSize the array is not a table!");
    }
  }
  getMultiSize(array[0], result);
}

bool operator < (const SizeOverIndex& left, const SizeOverIndex& right) {
  return left.sizeVal < right.sizeVal;
}

template <typename T>
void printMultiArray(const vector<T>& multiArray,
                     const vector<string>& dimNames) {
  vector<SizeOverIndex> multiSize;
  getMultiSize(multiArray, &multiSize);
  vector<int> multiIndex(multiSize.size(), 0);
  sort(multiSize.begin(), multiSize.end());
  reverse(multiSize.begin(), multiSize.end());
  printMultiArrayInner(&multiSize, dimNames, multiArray, &multiIndex);
}

template <typename T >
struct MultiArrayTraits {
  typedef T Type;
};

template <typename T>
struct MultiArrayTraits<vector<T> > {
  typedef typename MultiArrayTraits<T>::Type Type;
};

template <typename T>
typename MultiArrayTraits<T>::Type getValueByMultiIndex(
    vector<int>* const multiIndex,
    const vector<T>& multiArray) {
  if (multiIndex->size() != 1) {
    throw std::logic_error("Bad multiindex!");
  }
  return multiArray[multiIndex->back()];
}

template <typename T>
typename MultiArrayTraits<vector<T> >::Type
  getValueByMultiIndex(vector<int>* const multiIndex,
                        const vector<vector<T> >& multiArray) {
  int currentIndex = multiIndex->back();
  multiIndex->pop_back();
  typename MultiArrayTraits<vector<T> >::Type result =
      getValueByMultiIndex(multiIndex, multiArray[currentIndex]);
  multiIndex->push_back(currentIndex);
  return result;
}

template <typename T>
void printMultiArrayInner(vector<SizeOverIndex>* multiSize,
                          const vector<string>& dimNames,
                          const vector<T>& multiArray,
                          vector<int>* multiIndex) {
  if (multiSize->size() == 1) {
    int indexNoToIncrease = multiSize->back().indexVal;
    int sizeToPrint = multiSize->back().sizeVal;
    multiSize->pop_back();
    for (int i = 0; i < sizeToPrint; ++i) {
      (*multiIndex)[multiIndex->size() - 1 - indexNoToIncrease] = i;
      cout << getValueByMultiIndex(multiIndex, multiArray) << "\t";
    }
    SizeOverIndex backToPush = {indexNoToIncrease, sizeToPrint};
    multiSize->push_back(backToPush);
  } else if (multiSize->size() == 2) {
    int indexNoToIncrease = multiSize->back().indexVal;
    int sizeToPrint = multiSize->back().sizeVal;
    multiSize->pop_back();
    for (int i = 0; i < sizeToPrint; ++i) {
      (*multiIndex)[multiIndex->size() - 1 - indexNoToIncrease] = i;
      printMultiArrayInner(multiSize, dimNames, multiArray, multiIndex);
      cout << "\n";
    }
    SizeOverIndex backToPush = {indexNoToIncrease, sizeToPrint};
    multiSize->push_back(backToPush);
  } else {
    int indexNoToIncrease = multiSize->back().indexVal;
    int sizeToPrint = multiSize->back().sizeVal;
    multiSize->pop_back();
    for (int i = 0; i < sizeToPrint; ++i) {
      (*multiIndex)[multiIndex->size() - 1 - indexNoToIncrease] = i;
      printMultiArrayInner(multiSize, dimNames, multiArray, multiIndex);
      cout << "======================\n";
    }
    SizeOverIndex backToPush = {indexNoToIncrease, sizeToPrint};
    multiSize->push_back(backToPush);
  }
}

template <typename ObjectType, typename ScorerType>
class CrossValidator {
  vector<vector<vector<vector<double> > > > crossValidationResults_;
  vector<typename ltr::DataSet<ObjectType>::Ptr > dataSets_;
  vector<typename ltr::Measure<ObjectType>::Ptr> measures_;
  vector<typename ltr::Learner<ObjectType>::Ptr> learners_;
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
      const typename Learner<ObjectType>::Ptr& learner
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
              SplittedDataSet<ObjectType> splittedData(
                    splitters_[splitterIndex]->split(
                      splitIndex, *dataSets_[datasetIndex]));
              learners_[learnerIndex]->reset();
              learners_[learnerIndex]->learn(splittedData.train_set);

              Scorer::Ptr currentScorer = (
                    learners_[learnerIndex])->make();
              MarkDataSet(splittedData.test_set, *currentScorer);

              crossValidationResults_
                  [datasetIndex][measureIndex][learnerIndex]
                  [splitterIndex] +=
                  measures_[measureIndex]->average(splittedData.test_set) /
                  (*splitters_[splitterIndex]).splitCount(
                    *dataSets_[datasetIndex]);
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
    vector<SizeOverIndex> multiSize;
    getMultiSize(crossValidationResults_, &multiSize);
    sort(multiSize.begin(), multiSize.end());
    vector<string> dimNames;
    dimNames.push_back("dataSet");
    dimNames.push_back("learner");
    dimNames.push_back("splitter");
    dimNames.push_back("measure");
    printMultiArray(crossValidationResults_, dimNames);
  }
};
}
}

#endif  // LTR_CROSSVALIDATION_CROSSVALIDATION_RESULT_H_
