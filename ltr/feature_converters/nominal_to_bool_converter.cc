// Copyright 2012 Yandex

#include "ltr/feature_converters/nominal_to_bool_converter.h"

namespace ltr {
FeatureInfo NominalToBoolConverter::getNewFeatureInfo() const {
  FeatureInfo result;
  for (size_t i = 0; i < feature_info_.getFeatureCount(); i++)
    if (feature_info_.getFeatureType(i) != NOMINAL)
      result.addFeature(feature_info_.getFeatureType(i));

  for (size_t i = 0; i < feature_info_.getFeatureCount(); i++)
    if (feature_info_.getFeatureType(i) == NOMINAL) {
      int values_cnt = feature_info_.getFeatureValues(i).size();
      result.addFeature(BOOLEAN);
    }

  return result;
}

void NominalToBoolConverter::apply(
    const Object& argument, Object* value) const {
  *value = Object();
  for (size_t i = 0; i < argument.features().size(); i++)
    if (feature_info_.getFeatureType(i) != NOMINAL)
      *value << argument.features()[i];

  for (size_t i = 0; i < argument.features().size(); i++)
    if (feature_info_.getFeatureType(i) == NOMINAL) {
      map<unsigned int, string> vals = feature_info_.getFeatureValues(i);
      for (map<unsigned int, string>::iterator it = vals.begin();
           it != vals.end(); it++)
        if (argument.features()[i] == it->first)
          *value << 1.0;
        else
          *value << 0.0;
    }
}

string NominalToBoolConverter::generateCppCode(
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
    append("  vector<int> feature_values[").
    append(boost::lexical_cast<string>(feature_info_.getFeatureCount())).
    append("];\n");

  for (size_t i = 0; i < feature_info_.getFeatureCount(); i++)
    if (feature_info_.getFeatureType(i) == NOMINAL) {
      map<unsigned int, string> vals = feature_info_.getFeatureValues(i);
      for (map<unsigned int, string>::iterator it = vals.begin();
           it != vals.end(); it++) {
        hpp_string.
          append("  feature_values[").
          append(boost::lexical_cast<string>(i)).
          append("].push_back(").
          append(boost::lexical_cast<string>(it->first)).
          append(");\n");
      }
    }
  hpp_string.
    append("  for (size_t i = 0; i < features.size(); i++) {\n").
    append("    if (!nominal[i])\n").
    append("      result->push_back(features[i]);\n").
    append("  }\n");

  hpp_string.
    append("  for (size_t i = 0; i < features.size(); i++) {\n").
    append("    if (nominal[i]) {\n").
    append("      for (size_t j = 0; j < feature_values[i].size(); j++)\n").
    append("        result->push_back(features[i] == feature_values[i][j]);\n").
    append("    }\n").
    append("  }\n").
    append("}\n");
  return hpp_string;
}
}
