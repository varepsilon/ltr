// Copyright 2012 Yandex

#include "ltr/feature_converters/nan_to_zero_converter.h"
#include "ltr/utility/numerical.h"

using ltr::utility::isNaN;
namespace ltr {
FeatureInfo NanToZeroConverter::convertFeatureInfo() const  {
  return input_feature_info_;
}

string NanToZeroConverter::generateCppCode(
    const std::string &function_name) const {
  string code;
  code.
      append("#include <vector>\n\nvoid ").
      append(function_name).
      append("(const std::vector<double>& features, ").
      append("std::vector<double>* result) {\n").
      append("  result->clear();\n").
      append("  for (int i = 0; i < features.size(); ++i) {\n").
      append("    if (features[i] != features[i]) result->push_back(0);\n").
      append("    else result->push_back(features[i]);\n").
      append("  }\n").
      append("}\n");
  return code;
}

void NanToZeroConverter::applyImpl(
  const Object& input, Object* output) const {
  *output = input.deepCopy();
#pragma omp parallel for
  for (int feature_index = 0;
       feature_index < (int)input.features().size(); ++feature_index) {
    if (isNaN(input[feature_index])) {
      output->at(feature_index) = 0;
    }
  }
}

string NanToZeroConverter::getDefaultAlias() const {
  return "NanToZeroConverter";
}
};
