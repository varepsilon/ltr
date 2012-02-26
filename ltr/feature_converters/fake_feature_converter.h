// Copyright 2011 Yandex

#ifndef LTR_FEATURE_CONVERTERS_FAKE_FEATURE_CONVERTER_H_
#define LTR_FEATURE_CONVERTERS_FAKE_FEATURE_CONVERTER_H_

#include <boost/shared_ptr.hpp>

#include <string>

#include "ltr/data/object.h"
#include "ltr/data/feature_info.h"
#include "ltr/data/data_set.h"
#include "ltr/feature_converters/feature_converter.h"

using std::string;
using ltr::Object;

namespace ltr {

class FakeFeatureConverter : public FeatureConverter {
  public:
  typedef boost::shared_ptr<FakeFeatureConverter> Ptr;

  FakeFeatureConverter() {}

  FeatureInfo convertFeatureInfo(const FeatureInfo& oldFeatureInfo) const;

  void apply(const Object& source_object,
    Object* preprocessed_element) const;

  virtual string generateCppCode(
      const string& function_name) const {
    return "Not implemented.";
  }
};
}
#endif  // LTR_FEATURE_CONVERTERS_FAKE_FEATURE_CONVERTER_H_
