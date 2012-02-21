// Copyright 2011 Yandex

#ifndef LTR_FEATURE_CONVERTERS_FEATURE_SUBSET_CHOOSER_LEARNER_H_
#define LTR_FEATURE_CONVERTERS_FEATURE_SUBSET_CHOOSER_LEARNER_H_

#include <boost/shared_ptr.hpp>

#include <set>
#include <stdexcept>

#include "ltr/feature_converters/feature_converter_learner.h"
#include "ltr/feature_converters/feature_subset_chooser.h"

using std::set;
using std::logic_error;

namespace ltr {
template <typename TElement>
class FeatureSubsetChooserLearner : public IFeatureConverterLearner<TElement> {
  public:
  typedef boost::shared_ptr<FeatureSubsetChooserLearner> Ptr;

  explicit FeatureSubsetChooserLearner(const ParametersContainer& parameters =
      ParametersContainer()) {
    this->setDefaultParameters();
    this->parameters().copyParameters(parameters);
    this->checkParameters();
  }

  void learn(const DataSet<TElement>& data_set);
  FeatureConverter::Ptr make() const;

  void setDefaultParameters();
  void checkParameters() const;

  private:
  FeatureSubsetChooser::Ptr converter_;
};

// template realizations
template <typename TElement>
void FeatureSubsetChooserLearner<TElement>::setDefaultParameters() {
  vector<size_t> empty;
  this->parameters().setList("INDICES", empty);
}

template <typename TElement>
void FeatureSubsetChooserLearner<TElement>::checkParameters() const {
  vector<size_t> indices = this->parameters().getList("INDICES");
  set<size_t> used_objects;
  for (int index = 0; indices.size(); ++index) {
    size_t current_object = indices[index];
    if (used_obects.find(current_object) != used_obects.end()) {
      used_obects.insert(current_object);
    } else {
      throw logic_error("Indicies array contains equal elements");
    }
  }
}

template <typename TElement>
void FeatureSubsetChooserLearner<TElement>::learn(const DataSet<TElement>& data_set) {
  if (this->parameters().getList("INDICES").size() == 0) {
    vector<size_t> all_used(data_set.featureInfo().getFeatureCount());
    for (int index = 0; index < all_used.size(); ++index) {
      all_used[index] = index;
    }
    converter_.setChoosedFeaturesIndices(all_used);
  } else {
    converter_.setChoosedFeaturesIndices(this->parameters().getList("INDICES"));
  }
}

template <typename TElement>
FeatureSubsetChooser::Ptr FeatureSubsetChooserLearner<TElement>::make() const {
  return converter_;
}
};
#endif  // LTR_FEATURE_CONVERTERS_FEATURE_SUBSET_CHOOSER_LEARNER_H_
