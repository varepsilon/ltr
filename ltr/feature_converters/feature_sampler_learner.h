// Copyright 2011 Yandex

#ifndef LTR_FEATURE_CONVERTERS_FEATURE_SAMPLER_LEARNER_H_
#define LTR_FEATURE_CONVERTERS_FEATURE_SAMPLER_LEARNER_H_

#include <boost/shared_ptr.hpp>

#include <set>
#include <vector>
#include <stdexcept>
#include <string>
#include <sstream>

#include "ltr/feature_converters/feature_converter_learner.h"
#include "ltr/feature_converters/feature_sampler.h"

using std::string;
using std::set;
using std::logic_error;
using std::vector;

namespace ltr {
/**
 * Produces FeatureSampler with specific indices. Duplication of indices
 * is denied (throws while checking parameters)
 */
template <typename TElement>
class FeatureSamplerLearner
    : public FeatureConverterLearner<TElement, FeatureSampler> {
  friend class FeatureSampler;
 public:
  typedef boost::shared_ptr<FeatureSamplerLearner> Ptr;

  /**
   * @param parameters Standart LTR parameter container with list parameter
   * INDICES. INDICES is a list of indices of features to be used in produced
   * FeatureConverter. If INDICES is empty an equivalent to FakeFeatureConverter
   * for train dataset is produced.
   * By default INDICES is empty
   */
  explicit FeatureSamplerLearner(const ParametersContainer& parameters =
      ParametersContainer()) {
    this->setDefaultParameters();
    this->copyParameters(parameters);
    this->checkParameters();
  }

  void setDefaultParameters();
  void checkParameters() const;

  string toString() const;
 private:
  virtual void learnImpl(const DataSet<TElement>& data_set,
                         FeatureSampler *feature_sampler);
};

// template realizations
template <typename TElement>
void FeatureSamplerLearner<TElement>::setDefaultParameters() {
  this->clearParameters();
  vector<int> empty;
  this->addNewParam("INDICES", empty);
}

template <typename TElement>
void FeatureSamplerLearner<TElement>::checkParameters() const {
  vector<int> indices = this->parameters().
                        template Get<std::vector<int> >("INDICES");
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
string FeatureSamplerLearner<TElement>::toString() const {
  std::stringstream str;
    str << "Feature subset chooser feature converter learner"
      << "with parameter INDICES = {";
    vector<int> indices = this->parameters().template GetRef<std::vector<int> >(
                "INDICES");
    for (int i = 0; i < indices.size(); ++i) {
      if (i != 0) {
        str << ", ";
      }
      str << indices[i];
    }
    str << "}";
    return str.str();
}

template <typename TElement>
void FeatureSamplerLearner<TElement>::learnImpl(
    const DataSet<TElement>& data_set, FeatureSampler *feature_sampler) {
  const ParametersContainer &params = this->parameters();
  if (params.GetRef<std::vector<int> >("INDICES").size() == 0) {
    vector<int> all_used(data_set.featureInfo().get_feature_count());
    for (int index = 0; index < all_used.size(); ++index) {
      all_used[index] = index;
    }
    feature_sampler->setChoosedFeaturesIndices(all_used);
  } else {
    feature_sampler->setChoosedFeaturesIndices(
      params.GetRef<std::vector<int> >("INDICES"));
  }
}
};
#endif  // LTR_FEATURE_CONVERTERS_FEATURE_SAMPLER_LEARNER_H_
