// Copyright 2011 Yandex

#ifndef LTR_DATA_PREPROCESSORS_BEGGING_PREPROCESSOR_LEARNER_H_
#define LTR_DATA_PREPROCESSORS_BEGGING_PREPROCESSOR_LEARNER_H_

#include <boost/shared_ptr.hpp>

#include <ctime>
#include <cmath>
#include <vector>
#include <algorithm>


#include "ltr/data_preprocessors/data_preprocessor_learner.h"
#include "ltr/data_preprocessors/simple_subset_preprocessor.h"

using ltr::IDataPreprocessorLearner;
using std::vector;
using std::random_shuffle;
using std::copy;

namespace ltr {
template <typename TElement>
class BeggingPreprocessorLearner
    : public IDataPreprocessorLearner<TElement> {
  public:
  typedef boost::shared_ptr<BeggingPreprocessorLearner> Ptr;

  explicit BeggingPreprocessorLearner(
      const ParametersContainer& parameters = ParametersContainer())
      : preprocessor_(new SimpleSubsetPreprocessor<TElement>) {
    this->setDefaultParameters();
    this->copyParameters(parameters);
    this->checkParameters();
  }

  void learn(const DataSet<TElement>& data_set);
  typename DataPreprocessor<TElement>::Ptr make() const;

  void setDefaultParameters();
  void checkParameters() const;

  private:
  typename SimpleSubsetPreprocessor<TElement>::Ptr preprocessor_;
};

// template realizations
template <typename TElement>
void BeggingPreprocessorLearner<TElement>::setDefaultParameters() {
  this->clearParameters();
  this->addDoubleParameter("SELECTED_PART", 0.3);
  this->addBoolParameter("WITH_REPLACE", true);
}

template <typename TElement>
void BeggingPreprocessorLearner<TElement>::checkParameters() const {
  if (this->getBoolParameter("WITH_REPLACE")) {
    CHECK_DOUBLE_PARAMETER("SELECTED_PART", X >= 0);
  } else{
    CHECK_DOUBLE_PARAMETER("SELECTED_PART", X >= 0 && X <= 1);
  }
}

template <typename TElement>
void BeggingPreprocessorLearner<TElement>
    ::learn(const DataSet<TElement>& data_set) {
  int size = static_cast<int>(floor(data_set.size()
    * this->getDoubleParameter("SELECTED_PART")));

  if (size != 0) {
    vector<int> indices(size);
    srand(unsigned(time(NULL)));

    if (this->getBoolParameter("WITH_REPLACE")) {
      for (int i = 0; i < indices.size(); ++i) {
        indices[i] = (rand() % data_set.size());
      }
    } else {
      vector<int> all_used(data_set.size());
      for (int index = 0; index < all_used.size(); ++index) {
        all_used[index] = index;
      }
      random_shuffle(all_used.begin(), all_used.end());
      copy(all_used.begin(), all_used.begin() + size, indices.begin());
    }
    preprocessor_->setChoosedElementsIndices(indices);
  }
}

template <typename TElement>
typename DataPreprocessor<TElement>::Ptr
    BeggingPreprocessorLearner<TElement>::make() const {
  return preprocessor_;
}
};

#endif  // LTR_DATA_PREPROCESSORS_BEGGING_PREPROCESSOR_LEARNER_H_
