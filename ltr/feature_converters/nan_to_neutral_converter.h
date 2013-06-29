// Copyright 2012 Yandex

#ifndef LTR_FEATURE_CONVERTERS_NAN_TO_NEUTRAL_CONVERTER_H_
#define LTR_FEATURE_CONVERTERS_NAN_TO_NEUTRAL_CONVERTER_H_

#include <string>
#include <vector>

#include "ltr/feature_converters/feature_converter.h"

#include "ltr/utility/numerical.h"

#include "ltr/interfaces/parameterized.h"

using std::string;

using ltr::utility::isNaN;
using ltr::utility::lexical_cast;

namespace ltr {
/**
 * \brief Converts NaN features to average values.
 * 
 * Converts NaN features to average values of all
 * non NaN features from other Objects. If some feature is
 * NaN in all Objects this feature will be removed.
 * 
 * \param neutral_object_ object with neutral values
 */
class NanToNeutralConverter : public FeatureConverter {
  ALLOW_SHARED_PTR_ONLY_CREATION(NanToNeutralConverter)
 public:
  explicit NanToNeutralConverter(const ParametersContainer& parameters) {
  }

  NanToNeutralConverter(const FeatureInfo& feature_info = FeatureInfo())
    : FeatureConverter(feature_info) {}

  virtual FeatureInfo convertFeatureInfo() const;

  virtual string generateCppCode(const string& function_name) const;

  GET_SET(Object, neutral_object);

  virtual string toString() const;
 private:
  virtual void applyImpl(const Object& input, Object* output) const;

  virtual string getDefaultAlias() const;

  Object neutral_object_;
};
};
#endif  // LTR_FEATURE_CONVERTERS_NAN_TO_NEUTRAL_CONVERTER_H_
