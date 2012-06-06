// Copyright 2012 Yandex

#include "ltr/feature_converters/remove_nominal_converter.h"
#include "ltr/utility/numerical.h"

using ltr::utility::isNaN;
namespace ltr {

void RemoveNominalConverter::fillOutputFeatureInfo()  {
  output_feature_info_.clear();
  for (size_t i = 0; i < input_feature_info_.get_feature_count(); i++) {
    if (input_feature_info_.getFeatureType(i) != NOMINAL) {
      output_feature_info_.addFeature(input_feature_info_.getFeatureType(i));
    }
  }
}

void RemoveNominalConverter::applyImpl(
    const Object& argument, Object* value) const {
  *value = Object(output_feature_info());
  size_t result_idx = 0;
  for (size_t i = 0; i < argument.features().size(); i++)
    if (input_feature_info_.getFeatureType(i) != NOMINAL)
      value->features()[result_idx++] = argument.features()[i];
}

string RemoveNominalConverter::generateCppCode(
    const std::string &function_name) const {
  string hpp_string;
  hpp_string.
    append("#include <vector>\n\nvoid ").
    append(function_name).
    append("(const std::vector<double>& features, ").
    append("std::vector<double>* result) {\n").
    append("  result->clear();\n").
  append("  bool nominal[] = {");
  for (size_t i = 0; i < input_feature_info_.get_feature_count(); i++) {
    if (i != 0)
      hpp_string.append(",");
    if (input_feature_info_.getFeatureType(i) == NOMINAL)
      hpp_string.append("1");
    else
      hpp_string.append("0");
  }
  hpp_string.
    append("};\n").
    append("  for (size_t i = 0; i < features.size(); i++) {\n").
    append("    if (!nominal[i])\n").
    append("      result->push_back(features[i]);\n").
    append("  }\n").
    append("}\n");
  return hpp_string;
}
}
