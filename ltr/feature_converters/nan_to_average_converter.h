// Copyright 2012 Yandex

#ifndef LTR_FEATURE_CONVERTERS_NAN_TO_AVERAGE_CONVERTER_H_
#define LTR_FEATURE_CONVERTERS_NAN_TO_AVERAGE_CONVERTER_H_

#include <string>
#include <vector>

#include "ltr/feature_converters/feature_converter.h"
#include "ltr/feature_converters/feature_converter_learner.h"

#include "ltr/utility/numerical.h"

#include "ltr/interfaces/parameterized.h"

using ltr::utility::isNaN;

namespace ltr {
/**
 * Converts NaN features to average values of all
 * non NaN features from other Objects. If some feature is
 * NaN in all Objects this feature will be removed.
 */
class NanToAverageConverter : public FeatureConverter {
 public:
  typedef ltr::utility::shared_ptr<NanToAverageConverter> Ptr;

  NanToAverageConverter(const FeatureInfo& feature_info = FeatureInfo())
    : FeatureConverter(feature_info) {}

  virtual FeatureInfo convertFeatureInfo() const;

  virtual string generateCppCode(const string& function_name) const;

  GET_SET(vector<double>, average_features_values);

 private:
  virtual void applyImpl(const Object& input, Object* output) const;

  virtual string getDefaultAlias() const;

  vector<double> average_features_values_;
};
};
#endif  // LTR_FEATURE_CONVERTERS_NAN_TO_AVERAGE_CONVERTER_H_
