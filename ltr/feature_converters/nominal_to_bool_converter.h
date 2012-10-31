// Copyright 2012 Yandex

#ifndef LTR_FEATURE_CONVERTERS_NOMINAL_TO_BOOL_CONVERTER_H_
#define LTR_FEATURE_CONVERTERS_NOMINAL_TO_BOOL_CONVERTER_H_

#include <string>

#include "ltr/feature_converters/feature_converter.h"

namespace ltr {
/**
 * \brief Converts nominal features to boolean.
 *
 * Nominal feature with k values will be converted to k boolean features.
 */
class NominalToBoolConverter : public FeatureConverter {
 public:
  typedef ltr::utility::shared_ptr<NominalToBoolConverter> Ptr;

  explicit NominalToBoolConverter(const ParametersContainer& parameters) {
  }

  NominalToBoolConverter(const FeatureInfo& feature_info = FeatureInfo())
    : FeatureConverter(feature_info) {}

  virtual FeatureInfo convertFeatureInfo() const;

  string generateCppCode(const std::string& function_name) const;

  virtual string toString() const;
 private:
  virtual void applyImpl(const Object& input, Object* output) const;

  virtual string getDefaultAlias() const;
};
};
#endif  // LTR_FEATURE_CONVERTERS_NOMINAL_TO_BOOL_CONVERTER_H_
