// Copyright 2012 Yandex

#include <string>

#include "ltr/feature_converters/fake_feature_converter.h"

using std::string;

namespace ltr {
FeatureInfo FakeFeatureConverter::convertFeatureInfo() const {
  return input_feature_info_;
}

string FakeFeatureConverter::generateCppCode(
    const string& function_name) const {
  string code;
  code.
    append("#include <vector>\n\nvoid ").
    append(function_name).
    append("(const std::vector<double>& features, ").
    append("std::vector<double>* result) {").
    append("*result = features;}\n");
  return code;
}

void FakeFeatureConverter::applyImpl(
  const Object& input, Object* output) const {
    *output = input.deepCopy();
}

string FakeFeatureConverter::getDefaultAlias() const {
  return "FakeFeatureConverter";
}

string FakeFeatureConverter::toString() const {
  return "Fake feature converter";
}
};
