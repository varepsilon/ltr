// Copyright 2012 Yandex

#include "ltr/feature_converters/nominal_to_bool_converter.h"

namespace ltr {
FeatureInfo NominalToBoolConverter::convertFeatureInfo() const {
  FeatureInfo output_feature_info;
  for (int input_feature_index = 0;
       input_feature_index < input_feature_info_.feature_count();
       input_feature_index++) {
    if (input_feature_info_.getFeatureType(input_feature_index) != NOMINAL) {
      output_feature_info.addFeature(
        input_feature_info_.getFeatureType(input_feature_index));
    }
  }
  for (int input_feature_index = 0;
       input_feature_index < input_feature_info_.feature_count();
       input_feature_index++) {
    if (input_feature_info_.getFeatureType(input_feature_index) == NOMINAL) {
      output_feature_info.addFeature(BOOLEAN);
    }
  }
  return output_feature_info;
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
  for (int input_feature_index = 0;
       input_feature_index < input_feature_info_.feature_count();
       input_feature_index++) {
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
    append("  vector<int> feature_outputs[").
    append(
      boost::lexical_cast<string>(input_feature_info_.feature_count())).
    append("];\n");

  for (int input_feature_index = 0;
       input_feature_index < input_feature_info_.feature_count();
       input_feature_index++) {
    if (input_feature_info_.getFeatureType(input_feature_index) == NOMINAL) {
      map<int, string> vals =
        input_feature_info_.getFeatureValues(input_feature_index);
      for (map<int, string>::iterator iterator = vals.begin();
           iterator != vals.end(); ++iterator) {
        code.
          append("  feature_outputs[").
          append(boost::lexical_cast<string>(input_feature_index)).
          append("].push_back(").
          append(boost::lexical_cast<string>(iterator->first)).
          append(");\n");
      }
    }
  }
  code.
    append("  for (int i = 0; i < features.size(); ++i) {\n").
    append("    if (!nominal[i])\n").
    append("      result->push_back(features[i]);\n").
    append("  }\n");

  code.
    append("for (int i = 0; i < features.size(); ++i) {\n").
    append("  if (nominal[i]) {\n").
    append("    for (int j = 0; j < feature_outputs[i].size(); j++)\n").
    append("      result->push_back(features[i] == feature_outputs[i][j]);\n").
    append("  }\n").
    append("}\n").
    append("}\n");
  return code;
}

void NominalToBoolConverter::applyImpl(
  const Object& input, Object* output) const {
    Object converted_object;
    for (int input_feature_index = 0;
         input_feature_index < (int)input.features().size();
         ++input_feature_index) {
        if (input_feature_info_.getFeatureType(
            input_feature_index) != NOMINAL) {
          converted_object << input[input_feature_index];
        }
    }

    for (int input_feature_index = 0;
         input_feature_index < (int)input.features().size();
         ++input_feature_index) {
        if (input_feature_info_.getFeatureType(
            input_feature_index) == NOMINAL) {
          map<int, string> vals =
            input_feature_info_.getFeatureValues(input_feature_index);
          for (map<int, string>::iterator iterator = vals.begin();
            iterator != vals.end(); ++iterator) {
              if (input[input_feature_index] == iterator->first) {
                converted_object << 1.0;
              } else {
                converted_object << 0.0;
              }
          }
        }
    }
    converted_object.features().resize(input.feature_count());
    *output = converted_object;
}

string NominalToBoolConverter::getDefaultAlias() const {
  return "NominalToBoolConverter";
}
};
