// Copyright 2012 Yandex

#ifndef LTR_FEATURE_CONVERTERS_REMOVE_NAN_CONVERTER_H_
#define LTR_FEATURE_CONVERTERS_REMOVE_NAN_CONVERTER_H_

#include <string>

#include "ltr/feature_converters/feature_converter.h"

namespace ltr {
class RemoveNaNConverter : public FeatureConverter {
  public:
    typedef boost::shared_ptr<RemoveNaNConverter> Ptr;

    RemoveNaNConverter(const FeatureInfo& feature_info = FeatureInfo())
    : FeatureConverter(feature_info) {}

    virtual FeatureInfo getNewFeatureInfo() const;
    virtual void applyImpl(const Object& argument, Object* value) const;
    string generateCppCode(const std::string &) const;
};
}

#endif  // LTR_FEATURE_CONVERTERS_REMOVE_NAN_CONVERTER_H_
