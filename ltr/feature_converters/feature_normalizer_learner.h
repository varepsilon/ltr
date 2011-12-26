// Copyright 2011 Yandex

#ifndef LTR_FEATURE_CONVERTERS_FEATURE_NORMALIZER_LEARNER_H_
#define LTR_FEATURE_CONVERTERS_FEATURE_NORMALIZER_LEARNER_H_

#include <boost/shared_ptr.hpp>

#include <vector>
#include <algorithm>
#include <limits>

#include "ltr/feature_converters/feature_converter_learner.h"
#include "ltr/feature_converters/per_feature_linear_converter.h"
#include "ltr/utility/numerical.h"
#include "ltr/data/utility/data_set_statistics.h"

namespace ltr {

template <typename TElement>
class FeatureNormalizerLearner : public IFeatureConverterLearner<TElement> {
  public:
  typedef boost::shared_ptr< FeatureNormalizerLearner > Ptr;

  void learn(const DataSet<TElement>& data_set);
  FeatureConverter::Ptr make() const;

  private:
  void calcCurrentConverter();
  size_t featureCount() const;

  PerFeatureLinearConverter::Ptr current_converter_;
  std::vector<double> feature_min_statistic_;
  std::vector<double> feature_max_statistic_;
};

template <typename TElement>
void
FeatureNormalizerLearner<TElement>::learn(const DataSet<TElement>& data_set) {
  utility::calcMinMaxStatistics(data_set, &feature_min_statistic_,
      &feature_max_statistic_);
  this->calcCurrentConverter();
}

template <typename TElement>
size_t FeatureNormalizerLearner<TElement>::featureCount() const {
  return feature_min_statistic_.size();
}


template <typename TElement>
void FeatureNormalizerLearner<TElement>::calcCurrentConverter() {
  PerFeatureLinearConverter* p_converter = new PerFeatureLinearConverter(
      this->featureCount());
  for (size_t feature_idx = 0;
      feature_idx < this->featureCount();
      ++feature_idx) {
    double delta = feature_max_statistic_[feature_idx]
                                          - feature_min_statistic_[feature_idx];
    double coefficient;
    double shift;
    if (utility::DoubleEqual(delta, 0)) {
      coefficient = 0;
      shift = 0.5;
    } else {
      coefficient = 1.0 / delta;
      shift = -feature_min_statistic_[feature_idx] / delta;
    }
    p_converter->setCoefficient(feature_idx, coefficient);
    p_converter->setShift(feature_idx, shift);
  }
  current_converter_.reset(p_converter);
}

template <typename TElement>
FeatureConverter::Ptr FeatureNormalizerLearner<TElement>::make() const {
  return current_converter_;
}
}

#endif  // LTR_FEATURE_CONVERTERS_FEATURE_NORMALIZER_LEARNER_H_
