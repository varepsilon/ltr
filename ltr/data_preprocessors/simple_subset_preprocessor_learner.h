// Copyright 2011 Yandex

#ifndef LTR_DATA_PREPROCESSORS_SIMPLE_SUBSET_PREPROCESSOR_LEARNER_H_
#define LTR_DATA_PREPROCESSORS_SIMPLE_SUBSET_PREPROCESSOR_LEARNER_H_

#include <set>
#include <vector>

#include "ltr/data_preprocessors/data_preprocessor_learner.h"
#include "ltr/data_preprocessors/simple_subset_preprocessor.h"

using std::set;
using std::vector;
using ltr::DataPreprocessorLearner;

namespace ltr {
template <typename TElement>
class SimpleSubsetPreprocessorLearner
    : public DataPreprocessorLearner<TElement> {
  public:
  typedef boost::shared_ptr<SimpleSubsetPreprocessorLearner> Ptr;

  explicit SimpleSubsetPreprocessorLearner(
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
void SimpleSubsetPreprocessorLearner<TElement>::setDefaultParameters() {
  this->clearParameters();
  vector<int> empty;
  this->addListParameter("INDICES", empty);
}

template <typename TElement>
void SimpleSubsetPreprocessorLearner<TElement>::checkParameters() const {
  vector<int> indices = this->getListParameter("INDICES");
  set<int> used_elements;
  for (int index = 0; index < indices.size(); ++index) {
    int current_object = indices[index];
    if (used_elements.find(current_object) == used_elements.end()) {
      used_elements.insert(current_object);
    } else {
      throw logic_error("Indicies array contains equal elements");
    }
  }
}

template <typename TElement>
void SimpleSubsetPreprocessorLearner<TElement>
    ::learn(const DataSet<TElement>& data_set) {
  if (this->getListParameter("INDICES").size() == 0) {
    vector<int> all_used(data_set.size());
    for (int index = 0; index < all_used.size(); ++index) {
      all_used[index] = index;
    }
    preprocessor_->setChoosedElementsIndices(all_used);
  } else {
    preprocessor_->setChoosedElementsIndices(
      this->getListParameter("INDICES"));
  }
}

template <typename TElement>
typename DataPreprocessor<TElement>::Ptr
    SimpleSubsetPreprocessorLearner<TElement>::make() const {
  return preprocessor_;
}
};

#endif  // LTR_DATA_PREPROCESSORS_SIMPLE_SUBSET_PREPROCESSOR_LEARNER_H_
