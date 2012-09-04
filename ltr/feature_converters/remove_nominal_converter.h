// Copyright 2012 Yandex

#ifndef LTR_FEATURE_CONVERTERS_REMOVE_NOMINAL_CONVERTER_H_
#define LTR_FEATURE_CONVERTERS_REMOVE_NOMINAL_CONVERTER_H_

#include <string>

#include "ltr/feature_converters/feature_converter.h"

using std::string;

namespace ltr {
/**
 * \brief Remove all nominal features.
 */
class RemoveNominalConverter : public FeatureConverter {
 public:
  typedef ltr::utility::shared_ptr<RemoveNominalConverter> Ptr;

  RemoveNominalConverter(const FeatureInfo& feature_info = FeatureInfo())
    : FeatureConverter(feature_info) {}

  string generateCppCode(const std::string &function_name) const;

  virtual FeatureInfo convertFeatureInfo() const;

 private:
  virtual void applyImpl(const Object& input, Object* output) const;

  virtual string getDefaultAlias() const;
};
};
#endif  // LTR_FEATURE_CONVERTERS_REMOVE_NOMINAL_CONVERTER_H_
