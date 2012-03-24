// Copyright 2012 Yandex

#include "ltr/feature_converters/remove_nominal_converter.h"
#include "ltr/utility/numerical.h"

using ltr::utility::isNaN;
namespace ltr {

FeatureInfo RemoveNominalConverter::getNewFeatureInfo() const {
  FeatureInfo result;
  for (size_t i = 0; i < feature_info_.getFeatureCount(); i++)
    if (feature_info_.getFeatureType(i) != NOMINAL)
      result.addFeature(feature_info_.getFeatureType(i));
  return result;
}

void RemoveNominalConverter::apply(
    const Object& argument, Object* value) const {
  *value = Object();
  for (size_t i = 0; i < argument.features().size(); i++)
    if (feature_info_.getFeatureType(i) != NOMINAL)
      *value << argument.features()[i];
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
  for (size_t i = 0; i < feature_info_.getFeatureCount(); i++) {
    if (i != 0)
      hpp_string.append(",");
    if (feature_info_.getFeatureType(i) == NOMINAL)
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
