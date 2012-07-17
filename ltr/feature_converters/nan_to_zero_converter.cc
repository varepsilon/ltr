// Copyright 2012 Yandex

#include "ltr/feature_converters/nan_to_zero_converter.h"
#include "ltr/utility/numerical.h"

using ltr::utility::isNaN;
namespace ltr {

void NanToZeroConverter::fillOutputFeatureInfo()  {
  output_feature_info_ = input_feature_info_;
}

void NanToZeroConverter::applyImpl(const Object& input,
                                         Object* output) const {
  *output = input;
  for (size_t feature_index = 0;
      feature_index < input.features().size(); ++feature_index) {
    if (isNaN(input[feature_index])) {
      output->features()[feature_index] = 0;
    } else {
      output->features()[feature_index] = input[feature_index];
    }
  }
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
      append("  for (int i = 0; i < features.size(); i++) {\n").
      append("    if (features[i] != features[i]) result->push_back(0);\n").
      append("    else result->push_back(features[i]);\n").
      append("  }\n").
      append("}\n");
  return code;
}
}
