// Copyright 2012 Yandex

#ifndef LTR_FEATURE_CONVERTERS_NOMINAL_TO_BOOL_CONVERTER_H_
#define LTR_FEATURE_CONVERTERS_NOMINAL_TO_BOOL_CONVERTER_H_

#include <string>

#include "ltr/feature_converters/feature_converter.h"

namespace ltr {
class NominalToBoolConverter : public FeatureConverter {
  public:
    typedef boost::shared_ptr<NominalToBoolConverter> Ptr;

    NominalToBoolConverter(const FeatureInfo& feature_info = FeatureInfo())
    : FeatureConverter(feature_info) {}

    virtual FeatureInfo getNewFeatureInfo() const;
    virtual void apply(const Object& argument, Object* value) const;
    string generateCppCode(const std::string &) const;
};
}

#endif  // LTR_FEATURE_CONVERTERS_NOMINAL_TO_BOOL_CONVERTER_H_

