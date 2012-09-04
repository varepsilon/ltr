// Copyright 2012 Yandex

#include "ltr/feature_converters/remove_nominal_converter.h"
#include "ltr/utility/numerical.h"

using ltr::utility::isNaN;

namespace ltr {
FeatureInfo RemoveNominalConverter::convertFeatureInfo() const {
  FeatureInfo output_feature_info;
  for (int input_feature_index = 0;
       input_feature_index < input_feature_info_.feature_count();
       input_feature_index++) {
    if (input_feature_info_.getFeatureType(input_feature_index) != NOMINAL) {
      output_feature_info.addFeature(
        input_feature_info_.getFeatureType(input_feature_index));
    }
  }
  return output_feature_info;
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
  for (int input_feature_index = 0;
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
    append("  for (int i = 0; i < features.size(); ++i) {\n").
    append("    if (!nominal[i])\n").
    append("      result->push_back(features[i]);\n").
    append("  }\n").
    append("}\n");
  return code;
}

void RemoveNominalConverter::applyImpl(const Object& input,
                                             Object* output) const {
  Object converted_object = input.deepCopy();
  converted_object.features().clear();
  for (int input_feature_index = 0;
       input_feature_index < (int)input.features().size();
       ++input_feature_index) {
    if (input_feature_info_.getFeatureType(input_feature_index) != NOMINAL) {
      converted_object << input[input_feature_index];
    }
  }
  *output = converted_object;
}

string RemoveNominalConverter::getDefaultAlias() const {
  return "RemoveNominalConverter";
}
};
