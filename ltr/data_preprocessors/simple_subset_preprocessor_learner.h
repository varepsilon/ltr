// Copyright 2011 Yandex

#ifndef LTR_DATA_PREPROCESSORS_SIMPLE_SUBSET_PREPROCESSOR_LEARNER_H_
#define LTR_DATA_PREPROCESSORS_SIMPLE_SUBSET_PREPROCESSOR_LEARNER_H_

#include <set>
#include <vector>

#include "ltr/data_preprocessors/data_preprocessor_learner.h"
#include "ltr/data_preprocessors/simple_subset_preprocessor.h"

using std::set;
using std::vector;
using ltr::IDataPreprocessorLearner;

namespace ltr {
template <typename TElement>
class SimpleSubsetPreprocessorLearner
    : public IDataPreprocessorLearner<TElement> {
  public:
  typedef boost::shared_ptr<SimpleSubsetPreprocessorLearner> Ptr;

  explicit SimpleSubsetPreprocessorLearner(
      const ParametersContainer& parameters = ParametersContainer()) {
    this->setDefaultParameters();
    this->copyParameters(parameters);
    this->checkParameters();
  }

  void learn(const DataSet<TElement>& data_set);
  typename DataPreprocessor<TElement>::Ptr make() const;

  void setDefaultParameters();
  void checkParameters() const;

  private:
  typename SimpleSubsetPreprocessor<TElement>::Ptr converter_;
};

// template realizations
template <typename TElement>
void SimpleSubsetPreprocessorLearner<TElement>::setDefaultParameters() {
  vector<size_t> empty;
  this->setListParameter("INDICES", empty);
}

template <typename TElement>
void SimpleSubsetPreprocessorLearner<TElement>::checkParameters() const {
  vector<size_t> indices = this->getListParameter("INDICES");
  set<size_t> used_elements;
  for (int index = 0; indices.size(); ++index) {
    size_t current_object = indices[index];
    if (used_elements.find(current_object) != used_elements.end()) {
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
    vector<size_t> all_used(data_set.size());
    for (int index = 0; index < all_used.size(); ++index) {
      all_used[index] = index;
    }
    converter_->setChoosedElementsIndices(all_used);
  } else {
    converter_->setChoosedElementsIndices(
      this->getListParameter("INDICES"));
  }
}

template <typename TElement>
typename DataPreprocessor<TElement>::Ptr
    SimpleSubsetPreprocessorLearner<TElement>::make() const {
  return converter_;
}
};

#endif  // LTR_DATA_PREPROCESSORS_SIMPLE_SUBSET_PREPROCESSOR_LEARNER_H_
