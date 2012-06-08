// Copyright 2012 Yandex

#ifndef LTR_FEATURE_CONVERTERS_NAN_TO_ZERO_CONVERTER_H_
#define LTR_FEATURE_CONVERTERS_NAN_TO_ZERO_CONVERTER_H_

#include <string>

#include "ltr/feature_converters/feature_converter.h"
#include "ltr/feature_converters/feature_converter_wrapper.h"

namespace ltr {
class NanToZeroConverter : public FeatureConverter {
 public:
  typedef boost::shared_ptr<NanToZeroConverter> Ptr;

  NanToZeroConverter(const FeatureInfo& feature_info = FeatureInfo())
      : FeatureConverter(feature_info) {
    fillOutputFeatureInfo();
  }
  string generateCppCode(const std::string &) const;
 private:
  virtual void fillOutputFeatureInfo();
  virtual void applyImpl(const Object& argument, Object* value) const;
};

template <typename TElement>
class NanToZeroConverterLearner
    : public FeatureConverterWrapper<TElement, NanToZeroConverter> {
};
}

#endif  // LTR_FEATURE_CONVERTERS_NAN_TO_ZERO_CONVERTER_H_
