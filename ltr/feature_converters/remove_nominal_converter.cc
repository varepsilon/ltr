// Copyright 2012 Yandex

#include "ltr/feature_converters/remove_nominal_converter.h"
#include "ltr/utility/numerical.h"

using ltr::utility::isNaN;

namespace ltr {
void RemoveNominalConverter::fillOutputFeatureInfo()  {
  output_feature_info_.clear();
  for (size_t input_feature_index = 0;
       input_feature_index < input_feature_info_.feature_count();
       input_feature_index++) {
    if (input_feature_info_.getFeatureType(input_feature_index) != NOMINAL) {
      output_feature_info_.addFeature(
        input_feature_info_.getFeatureType(input_feature_index));
    }
  }
}

void RemoveNominalConverter::applyImpl(const Object& input,
                                             Object* output) const {
  *output = Object(output_feature_info_);
  output->features().resize(output_feature_info_.feature_count());
  size_t output_feature_index = 0;
  for (size_t input_feature_index = 0;
       input_feature_index < input.features().size();
       ++input_feature_index) {
    if (input_feature_info_.getFeatureType(input_feature_index) != NOMINAL) {
      output->features()[output_feature_index++]
          = input[input_feature_index];
    }
  }
}

string RemoveNominalConverter::generateCppCode(
    const string &function_name) const {
  string code;
  code.
    append("#include <vector>\n\nvoid ").
    append(function_name).
    append("(const std::vector<double>& features, ").
    append("std::vector<double>* result) {\n").
    append("  result->clear();\n").
  append("  bool nominal[] = {");
  for (size_t input_feature_index = 0;
       input_feature_index < input_feature_info_.feature_count();
       ++input_feature_index) {
    if (input_feature_index != 0) {
      code.append(",");
    }
    if (input_feature_info_.getFeatureType(input_feature_index) == NOMINAL) {
      code.append("1");
    } else {
      code.append("0");
    }
  }
  code.
    append("};\n").
    append("  for (size_t i = 0; i < features.size(); ++i) {\n").
    append("    if (!nominal[i])\n").
    append("      result->push_back(features[i]);\n").
    append("  }\n").
    append("}\n");
  return code;
}
}
