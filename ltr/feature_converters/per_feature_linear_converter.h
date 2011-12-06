// Copyright 2011 Yandex

#ifndef LTR_FEATURE_CONVERTERS_PER_FEATURE_LINEAR_CONVERTER_H_
#define LTR_FEATURE_CONVERTERS_PER_FEATURE_LINEAR_CONVERTER_H_

#include <boost/shared_ptr.hpp>

#include <vector>

#include "ltr/feature_converters/feature_converter.h"

namespace ltr {

class PerFeatureLinearConverter : public FeatureConverter {
  public:
  typedef boost::shared_ptr< PerFeatureLinearConverter > Ptr;

  explicit PerFeatureLinearConverter(size_t featureCount) :
    coefficient_(featureCount, 1),
    shift_(featureCount, 0) {}

  FeatureInfo convertFeatureInfo(const FeatureInfo& oldFeatureInfo) const;

  double getCoefficient(size_t feature_idx) const;
  void setCoefficient(size_t feature_idx, double coefficient);
  double getShift(size_t feature_idx) const;
  void setShift(size_t feature_idx, double shift);

  void apply(const ltr::Object & argument, ltr::Object * value) const;

  private:
  size_t featureCount() const;
  void checkFeatureCount(size_t checkedFeatureCount) const;

  std::vector<double> coefficient_;
  std::vector<double> shift_;
};
}

#endif  // LTR_FEATURE_CONVERTERS_PER_FEATURE_LINEAR_CONVERTER_H_
