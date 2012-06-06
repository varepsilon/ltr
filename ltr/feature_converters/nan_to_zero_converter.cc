// Copyright 2012 Yandex

#include "ltr/feature_converters/nan_to_zero_converter.h"
#include "ltr/utility/numerical.h"

using ltr::utility::isNaN;
namespace ltr {

void NanToZeroConverter::fillOutputFeatureInfo()  {
  output_feature_info_ = input_feature_info_;
}

void NanToZeroConverter::applyImpl(const Object& argument,
                                   Object* value) const {
  *value = Object(output_feature_info());
  for (size_t i = 0; i < argument.features().size(); i++) {
    if (isNaN(argument.features()[i]))
      value->features()[i] = 0;
    else
      value->features()[i] = argument.features()[i];
  }
}

string NanToZeroConverter::generateCppCode(
    const std::string &function_name) const {
  string hpp_string;
  hpp_string.
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
  return hpp_string;
}
}
