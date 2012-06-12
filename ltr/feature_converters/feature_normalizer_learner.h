// Copyright 2011 Yandex

#ifndef LTR_FEATURE_CONVERTERS_FEATURE_NORMALIZER_LEARNER_H_
#define LTR_FEATURE_CONVERTERS_FEATURE_NORMALIZER_LEARNER_H_

#include <boost/shared_ptr.hpp>

#include <vector>
#include <algorithm>
#include <limits>
#include <string>
#include <sstream>

#include "ltr/feature_converters/feature_converter_learner.h"
#include "ltr/feature_converters/per_feature_linear_converter.h"
#include "ltr/utility/numerical.h"
#include "ltr/data/utility/data_set_statistics.h"

using std::vector;
using std::string;
using std::logic_error;

using ltr::utility::getFeaturesMinMaxValues;

namespace ltr {

/**
* \brief Independently normalize each feature component to the specified range.
* \param min desired minimum values
* \param max desired maximum values
*/
template <class TElement>
class FeatureNormalizerLearner
    : public BaseFeatureConverterLearner<TElement, PerFeatureLinearConverter> {
 public:
  typedef boost::shared_ptr<FeatureNormalizerLearner> Ptr;

  /**
  * \param min desired minimum values
  * \param max desired maximum values
  */
  explicit FeatureNormalizerLearner(double min = 0.0, double max = 1.0);
  explicit FeatureNormalizerLearner(const ParametersContainer& parameters);

  virtual void setDefaultParameters();
  virtual void checkParameters() const;

  virtual string toString() const;

  GET_SET(double, min);
  GET_SET(double, max);
 private:
  virtual void learnImpl(const DataSet<TElement>& data_set,
      PerFeatureLinearConverter* per_feature_linear_converter);
  virtual void setParametersImpl(const ParametersContainer& parameters);

  double min_;
  double max_;
};

template <typename TElement>
FeatureNormalizerLearner<TElement>::FeatureNormalizerLearner(double min,
                                                             double max) {
  min_ = min;
  max_ = max;
}

template <typename TElement>
FeatureNormalizerLearner<TElement>::FeatureNormalizerLearner(
    const ParametersContainer& parameters) {
  this->setParameters(parameters);
}

template <typename TElement>
void FeatureNormalizerLearner<TElement>::learnImpl(
    const DataSet<TElement>& data_set,
    PerFeatureLinearConverter* per_feature_linear_converter) {
  per_feature_linear_converter->resize(data_set.feature_info());
  Features input_min, input_max;
  getFeaturesMinMaxValues(data_set, &input_min, &input_max);

  for (size_t feature_index = 0;
      feature_index < data_set.feature_count(); ++feature_index) {
    double delta = input_max[feature_index] - input_min[feature_index];
    double coefficient, shift;
    if (utility::DoubleEqual(delta, 0)) {
      coefficient = 0.0;
      shift = (max_ - min_) / 2;
    } else {
      coefficient = (max_ - min_) / delta;
      shift = min_ - coefficient * input_min[feature_index];
    }
    per_feature_linear_converter->set_factor(feature_index, coefficient);
    per_feature_linear_converter->set_shift(feature_index, shift);
  }
}

template <typename TElement>
string FeatureNormalizerLearner<TElement>::toString() const {
  std::stringstream str;
  std::fixed(str);
  str.precision(2);
  str << "FeatureNormalizerLearner:";
  str << " min = " << min_;
  str << ", max = " << max_;
  return str.str();
}

template <typename TElement>
void FeatureNormalizerLearner<TElement>::setParametersImpl(
    const ParametersContainer& parameters) {
  min_ = parameters.Get<double>("MIN");
  max_ = parameters.Get<double>("MAX");
}


template <typename TElement>
void FeatureNormalizerLearner<TElement>::setDefaultParameters() {
  min_ = 0.0;
  max_ = 1.0;
}

template <typename TElement>
void FeatureNormalizerLearner<TElement>::checkParameters() const {
  CHECK(min_ < max_);
}
}
#endif  // LTR_FEATURE_CONVERTERS_FEATURE_NORMALIZER_LEARNER_H_
