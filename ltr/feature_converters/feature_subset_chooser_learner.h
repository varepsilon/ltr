// Copyright 2011 Yandex

#ifndef LTR_FEATURE_CONVERTERS_FEATURE_SUBSET_CHOOSER_LEARNER_H_
#define LTR_FEATURE_CONVERTERS_FEATURE_SUBSET_CHOOSER_LEARNER_H_

#include <boost/shared_ptr.hpp>

#include <set>
#include <vector>
#include <stdexcept>

#include "ltr/feature_converters/feature_converter_learner.h"
#include "ltr/feature_converters/feature_subset_chooser.h"

using std::set;
using std::logic_error;
using std::vector;

namespace ltr {
template <typename TElement>
class FeatureSubsetChooserLearner : public IFeatureConverterLearner<TElement> {
  public:
  typedef boost::shared_ptr<FeatureSubsetChooserLearner> Ptr;

  explicit FeatureSubsetChooserLearner(const ParametersContainer& parameters =
      ParametersContainer())
      : converter_(new FeatureSubsetChooser) {
    this->setDefaultParameters();
    this->copyParameters(parameters);
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
  vector<int> empty;
  this->addListParameter("INDICES", empty);
}

template <typename TElement>
void FeatureSubsetChooserLearner<TElement>::checkParameters() const {
  vector<int> indices = this->getListParameter("INDICES");
  set<int> used_features;
  for (int index = 0; index < indices.size(); ++index) {
    int current_object = indices[index];
    if (used_features.find(current_object) == used_features.end()) {
      used_features.insert(current_object);
    } else {
      throw logic_error("Indicies array contains equal elements");
    }
  }
}

template <typename TElement>
void FeatureSubsetChooserLearner<TElement>
    ::learn(const DataSet<TElement>& data_set) {
  if (this->getListParameter("INDICES").size() == 0) {
    vector<int> all_used(data_set.featureInfo().getFeatureCount());
    for (int index = 0; index < all_used.size(); ++index) {
      all_used[index] = index;
    }
    converter_->setChoosedFeaturesIndices(all_used);
  } else {
    converter_->setChoosedFeaturesIndices(
      this->getListParameter("INDICES"));
  }
}

template <typename TElement>
FeatureConverter::Ptr FeatureSubsetChooserLearner<TElement>::make() const {
  return converter_;
}
};
#endif  // LTR_FEATURE_CONVERTERS_FEATURE_SUBSET_CHOOSER_LEARNER_H_
