// Copyright 2012 Yandex

#ifndef LTR_FEATURE_CONVERTERS_FEATURE_RANDOM_SAMPLER_LEARNER_H_
#define LTR_FEATURE_CONVERTERS_FEATURE_RANDOM_SAMPLER_LEARNER_H_

#include <ctime>
#include <cmath>
#include <functional>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>

#include "ltr/feature_converters/feature_converter_learner.h"
#include "ltr/feature_converters/feature_sampler.h"

#include "ltr/utility/indices.h"
#include "ltr/utility/random_number_generator.h"
#include "ltr/utility/shared_ptr.h"

using std::string;
using std::vector;

using ltr::utility::Indices;
using ltr::utility::getRandomIndices;
using ltr::utility::randomizer;

namespace ltr {
/**
 * \brief Produces FeatureSampler with random indices
 * \param sampling_fraction portion of features to sample, must be in (0,1]
 * \param seed seed for random numbers generator
 */
template <typename TElement>
class FeatureRandomSamplerLearner
    : public BaseFeatureConverterLearner<TElement, FeatureSampler> {
  friend class FeatureSampler;

 public:
  typedef ltr::utility::shared_ptr<FeatureRandomSamplerLearner> Ptr;
  /**
   * \param sampling_fraction portion of features to sample, must be in (0,1]
   * \param seed seed for random numbers generator
   */
  explicit FeatureRandomSamplerLearner(double sampling_fraction = 0.3,
                                       int seed = 42)
  : sampling_fraction_(sampling_fraction),
    seed_(seed) {}

  explicit FeatureRandomSamplerLearner(const ParametersContainer& parameters);

  virtual void setDefaultParameters();

  virtual void checkParameters() const;

  virtual string toString() const;

  GET_SET(double, sampling_fraction);
  GET(int, seed);

  void set_seed(int seed);

 private:
  virtual void learnImpl(const DataSet<TElement>& data_set,
                         FeatureSampler* feature_sampler);

  virtual void setParametersImpl(const ParametersContainer& parameters);

  virtual string getDefaultAlias() const;

  double sampling_fraction_;
  int seed_;
};

// template realizations

template <class TElement>
FeatureRandomSamplerLearner<TElement>::FeatureRandomSamplerLearner(
    const ParametersContainer& parameters) {
  this->setParameters(parameters);
}

template <class TElement>
void FeatureRandomSamplerLearner<TElement>::setDefaultParameters() {
  sampling_fraction_ = 0.3;
  set_seed(42);
}

template <class TElement>
void FeatureRandomSamplerLearner<TElement>::checkParameters() const {
  CHECK(sampling_fraction_ > 0. && sampling_fraction_ <= 1.);
  CHECK(seed_ >= 0);
}

template <typename TElement>
string FeatureRandomSamplerLearner<TElement>::toString() const {
  std::stringstream str;
  std::fixed(str);
  str.precision(2);
  str << "FeatureRandomSamplerLearner: sampling_fraction = ";
  str << sampling_fraction_ << ", seed = " << seed_;
  return str.str();
}

template <typename TElement>
void FeatureRandomSamplerLearner<TElement>::set_seed(int seed) {
  CHECK(seed >= 0);
  seed_ = seed;
  randomizer.setSeed(seed);
}

template <typename TElement>
void FeatureRandomSamplerLearner<TElement>::learnImpl(
    const DataSet<TElement>& data_set, FeatureSampler* feature_sampler) {
  int sample_size = (int)ceil(data_set.feature_count() * sampling_fraction_);
  Indices indices;
  getRandomIndices(&indices, data_set.feature_count(), sample_size);
  feature_sampler->set_indices(indices);
}

template <class TElement>
void FeatureRandomSamplerLearner<TElement>::setParametersImpl(
    const ParametersContainer& parameters) {
  sampling_fraction_ = parameters.Get<double>("SAMPLING_FRACTION");
  set_seed(parameters.Get<int>("SEED"));
}

template <class TElement>
string FeatureRandomSamplerLearner<TElement>::getDefaultAlias() const {
  return "FeatureRandomSamplerLearner";
}
};
#endif  // LTR_FEATURE_CONVERTERS_FEATURE_RANDOM_SAMPLER_LEARNER_H_
