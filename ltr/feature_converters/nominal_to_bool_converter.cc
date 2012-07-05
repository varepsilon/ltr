// Copyright 2012 Yandex

#include "ltr/feature_converters/nominal_to_bool_converter.h"

namespace ltr {
void NominalToBoolConverter::fillOutputFeatureInfo()  {
  output_feature_info_.clear();
  for (size_t i = 0; i < input_feature_info_.feature_count(); i++) {
    if (input_feature_info_.getFeatureType(i) != NOMINAL) {
      output_feature_info_.addFeature(input_feature_info_.getFeatureType(i));
    }
  }
  for (size_t i = 0; i < input_feature_info_.feature_count(); i++) {
    if (input_feature_info_.getFeatureType(i) == NOMINAL) {
      // \FIXME(sameg) Seems like bug!
      int outputs_cnt = input_feature_info_.getFeatureValues(i).size();
      output_feature_info_.addFeature(BOOLEAN);
    }
  }
}

void NominalToBoolConverter::applyImpl(
    const Object& input, Object* output) const {
  *output = input;
  size_t result_idx = 0;
  for (size_t i = 0; i < input.features().size(); ++i)
    if (input_feature_info_.getFeatureType(i) != NOMINAL)
      output->features()[result_idx++] = input.features()[i];

  for (size_t i = 0; i < input.features().size(); ++i)
    if (input_feature_info_.getFeatureType(i) == NOMINAL) {
      map<size_t, string> vals = input_feature_info_.getFeatureValues(i);
      for (map<size_t, string>::iterator it = vals.begin();
           it != vals.end(); ++it)
        if (input.features()[i] == it->first) {
          output->features()[result_idx++] = 1.0;
        } else {
          output->features()[result_idx++] = 0.0;
        }
    }
}

string NominalToBoolConverter::generateCppCode(
    const std::string &function_name) const {
  string code;
  code.
    append("#include <vector>\n\nvoid ").
    append(function_name).
    append("(const std::vector<double>& features, ").
    append("std::vector<double>* result) {\n").
    append("  result->clear();\n").
    append("  bool nominal[] = {");
  for (size_t i = 0; i < input_feature_info_.feature_count(); i++) {
    if (i != 0)
      code.append(",");
    if (input_feature_info_.getFeatureType(i) == NOMINAL)
      code.append("1");
    else
      code.append("0");
  }
  code.
    append("};\n").
    append("  vector<int> feature_outputs[").
    append(
      boost::lexical_cast<string>(input_feature_info_.feature_count())).
    append("];\n");

  for (size_t i = 0; i < input_feature_info_.feature_count(); i++)
    if (input_feature_info_.getFeatureType(i) == NOMINAL) {
      map<size_t, string> vals = input_feature_info_.getFeatureValues(i);
      for (map<size_t, string>::iterator it = vals.begin();
           it != vals.end(); it++) {
        code.
          append("  feature_outputs[").
          append(boost::lexical_cast<string>(i)).
          append("].push_back(").
          append(boost::lexical_cast<string>(it->first)).
          append(");\n");
      }
    }
  code.
    append("  for (size_t i = 0; i < features.size(); i++) {\n").
    append("    if (!nominal[i])\n").
    append("      result->push_back(features[i]);\n").
    append("  }\n");

  code.
    append("for (size_t i = 0; i < features.size(); i++) {\n").
    append("  if (nominal[i]) {\n").
    append("    for (size_t j = 0; j < feature_outputs[i].size(); j++)\n").
    append("      result->push_back(features[i] == feature_outputs[i][j]);\n").
    append("  }\n").
    append("}\n").
    append("}\n");
  return code;
}
}
