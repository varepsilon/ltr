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

using std::vector;
using std::random_shuffle;
using std::copy;

namespace ltr {
/**
 * Produces SimpleSubsetPreprocessor with random indices (duplication allowed)
 */
template <typename TElement>
class BeggingPreprocessorLearner
    : public DataPreprocessorLearner<TElement, SimpleSubsetPreprocessor> {
  public:
  typedef boost::shared_ptr<BeggingPreprocessorLearner> Ptr;

  /**
   * @param parameters Standart LTR parameter container with double parameter
   * SELECTED_PART, bool parameter WITH_REPLACE and int parameter RANDOM_SEED.
   * WITH_REPLACE is true if dublication in indices of produced SimpleSubset
   * Preprocessor is allowed, false otherwise. SELECTED_PART is a part of elements
   * chosen by produced SimpleSubsetPreprocessor. Upper rounding is used, so
   * never produces SimpleSubsetPreprocessor with 0 features. RANDOM_SEED is for
   * manual control of random behavior of BeggingPreprocessorLearner
   * By default SELECTED_PART = 0.3, WITH_REPLACE = true, RANDOM_SEED = 237
   */
  explicit BeggingPreprocessorLearner(
      const ParametersContainer& parameters = ParametersContainer()) {
    this->setDefaultParameters();
    this->copyParameters(parameters);
    this->checkParameters();
    srand(this->getIntParameter("RANDOM_SEED"));
  }

  void learn(const DataSet<TElement>& data_set);
  SimpleSubsetPreprocessor<TElement> make() const;

  void setDefaultParameters();
  void checkParameters() const;

  private:
    SimpleSubsetPreprocessor<TElement> preprocessor_;
};

// template realizations
template <typename TElement>
void BeggingPreprocessorLearner<TElement>::setDefaultParameters() {
  this->clearParameters();
  this->addDoubleParameter("SELECTED_PART", 0.3);
  this->addBoolParameter("WITH_REPLACE", true);
  this->addIntParameter("RANDOM_SEED", 237);
}

template <typename TElement>
void BeggingPreprocessorLearner<TElement>::checkParameters() const {
  if (this->getBoolParameter("WITH_REPLACE")) {
    CHECK_DOUBLE_PARAMETER("SELECTED_PART", X > 0);
  } else {
    CHECK_DOUBLE_PARAMETER("SELECTED_PART", X > 0 && X <= 1);
  }
  CHECK_INT_PARAMETER("RANDOM_SEED", X > 0);
}

template <typename TElement>
void BeggingPreprocessorLearner<TElement>
    ::learn(const DataSet<TElement>& data_set) {
  int size = static_cast<int>(ceil(data_set.size()
    * this->getDoubleParameter("SELECTED_PART")));

  if (size != 0) {
    vector<int> indices(size);

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
    preprocessor_.setChoosedElementsIndices(indices);
  }
}

template <typename TElement>
SimpleSubsetPreprocessor<TElement>
    BeggingPreprocessorLearner<TElement>::make() const {
  return preprocessor_;
}
};

#endif  // LTR_DATA_PREPROCESSORS_BEGGING_PREPROCESSOR_LEARNER_H_
