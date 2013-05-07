// Copyright 2012 Yandex

#include <string>
#include <sstream>

#include "ltr/feature_converters/fake_feature_converter.h"

using std::string;
using std::stringstream;

namespace ltr {
FeatureInfo FakeFeatureConverter::convertFeatureInfo() const {
  return input_feature_info_;
}

string FakeFeatureConverter::generateCppCode(
    const string& function_name) const {
  stringstream code;
  code
    << "#include <vector>\n"
    << "\n"
    << "void " << function_name << "(const std::vector<double>& features,\n"
    << "    std::vector<double>* result) {\n"
    << "  *result = features;\n"
    << "}\n";
  return code.str();
}

void FakeFeatureConverter::applyImpl(
  const Object& input, Object* output) const {
    output->set_eigen_features(input.eigen_features());
}

string FakeFeatureConverter::getDefaultAlias() const {
  return "FakeFeatureConverter";
}

string FakeFeatureConverter::toString() const {
  return "Fake feature converter";
}
};
