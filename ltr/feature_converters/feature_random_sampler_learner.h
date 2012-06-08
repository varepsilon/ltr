// Copyright 2011 Yandex

#ifndef LTR_FEATURE_CONVERTERS_FEATURE_RANDOM_SAMPLER_LEARNER_H_
#define LTR_FEATURE_CONVERTERS_FEATURE_RANDOM_SAMPLER_LEARNER_H_

#include <boost/shared_ptr.hpp>

#include <ctime>
#include <cmath>
#include <functional>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>

#include "ltr/feature_converters/feature_converter_learner.h"
#include "ltr/feature_converters/feature_sampler.h"

using std::string;
using std::vector;
using std::random_shuffle;
using std::copy;

namespace ltr {
/**
 * Produces FeatureSampler with random indices (with no duplication)
 */
template <typename TElement>
class FeatureRandomSamplerLearner
    : public FeatureConverterLearner<TElement, FeatureSampler> {
  friend FeatureSampler;
 public:
  typedef boost::shared_ptr<FeatureRandomSamplerLearner> Ptr;

  /**
   * @param parameters Standart LTR parameter container with double parameter
   * SELECTED_PART and int parameter RANDOM_SEED. SELECTED_PART is a part of
   * features chosen by produced FeatureSampler. Upper rounding is used,
   * so never produces FeatureSampler with 0 features. RANDOM_SEED is for
   * manual control of random behavior of FeatureRandomSamplerLearner
   * By default SELECTED_PART = 0.3, RANDOM_SEED = 1339
   */
  explicit FeatureRandomSamplerLearner(const ParametersContainer& parameters =
      ParametersContainer()) {
    this->setDefaultParameters();
    this->copyParameters(parameters);
    this->checkParameters();
    srand(this->parameters().template Get<int>("RANDOM_SEED"));
  }

  virtual void learn(const DataSet<TElement>& data_set);
  virtual FeatureSampler::Ptr makeSpecific() const;

  void setDefaultParameters();
  void checkParameters() const;

  string toString() const;
 private:
  template <class T>
  struct Belongs: public std::unary_function<T, bool> {
    Belongs(const T &min, const T &max): min_(min), max_(max) { }
    bool operator()(const T& x) const {
      return x > min_ && x <= max_;
    }
   private:
    const T &min_;
    const T &max_;
  };

  FeatureSampler::Ptr converter_;
};

// template realizations
template <typename TElement>
void FeatureRandomSamplerLearner<TElement>::setDefaultParameters() {
  this->clearParameters();
  this->addNewParam("SELECTED_PART", 0.3);
  this->addNewParam("RANDOM_SEED", 1339);
}

template <typename TElement>
void FeatureRandomSamplerLearner<TElement>::checkParameters() const {
  const Belongs<double> G0L1(0, 1);
  Parameterized::checkParameter<double>("SELECTED_PART", Belongs<double>(0, 1));
  Parameterized::checkParameter<int>("RANDOM_SEED",
                                     std::bind2nd(std::greater<int>(), 0));
}

template <typename TElement>
string FeatureRandomSamplerLearner<TElement>::toString() const {
  std::stringstream str;
  std::fixed(str);
  str.precision(2);
  str << "RSM feature converter learner with parameters: SELECTED_PART = ";
  str << this->parameters().template Get<double>("SELECTED_PART");
  str << ", RANDOM_SEED = ";
  str << this->parameters().template Get<int>("RANDOM_SEED");
  return str.str();
}

template <typename TElement>
void FeatureRandomSamplerLearner<TElement>::learn(
    const DataSet<TElement>& data_set) {
  converter_ = FeatureSampler::Ptr(new FeatureSampler);
  const ParametersContainer &params = this->parameters();
  converter_->set_input_feature_info(data_set.featureInfo());
  int size = static_cast<int>(
    ceil(data_set.featureInfo().get_feature_count()
      * params.Get<double>("SELECTED_PART")));
  vector<int> indices(size);

  vector<int> all_used(data_set.featureInfo().get_feature_count());
  for (int index = 0; index < all_used.size(); ++index) {
    all_used[index] = index;
  }
  random_shuffle(all_used.begin(), all_used.end());
  copy(all_used.begin(), all_used.begin() + size, indices.begin());
  converter_->setChoosedFeaturesIndices(indices);
}

template <typename TElement>
FeatureSampler::Ptr
FeatureRandomSamplerLearner<TElement>::makeSpecific() const {
  return converter_;
}
};
#endif  // LTR_FEATURE_CONVERTERS_FEATURE_RANDOM_SAMPLER_LEARNER_H_
