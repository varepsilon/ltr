// Copyright 2012 Yandex

#ifndef LTR_FEATURE_CONVERTERS_FAKE_FEATURE_CONVERTER_H_
#define LTR_FEATURE_CONVERTERS_FAKE_FEATURE_CONVERTER_H_

#include <string>

#include "ltr/data/object.h"
#include "ltr/data/feature_info.h"
#include "ltr/data/data_set.h"

#include "ltr/feature_converters/feature_converter.h"

#include "ltr/utility/shared_ptr.h"

using std::string;

namespace ltr {
/**
 * Fake feature converter. Does nothing with input object
 */
class FakeFeatureConverter : public FeatureConverter {
 public:
  typedef ltr::utility::shared_ptr<FakeFeatureConverter> Ptr;

  FakeFeatureConverter(const FeatureInfo& feature_info = FeatureInfo())
    : FeatureConverter(feature_info) {
    fillOutputFeatureInfo();
  }

  virtual void fillOutputFeatureInfo();

  virtual string generateCppCode(const string& function_name) const;

 private:
  virtual void applyImpl(const Object& input,
                               Object* output) const;

  virtual string getDefaultAlias() const;
};
};
#endif  // LTR_FEATURE_CONVERTERS_FAKE_FEATURE_CONVERTER_H_
