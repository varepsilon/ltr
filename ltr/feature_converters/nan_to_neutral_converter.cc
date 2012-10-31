// Copyright 2012 Yandex

#include "boost/lexical_cast.hpp"

#include "ltr/feature_converters/nan_to_neutral_converter.h"

namespace ltr {
FeatureInfo NanToNeutralConverter::convertFeatureInfo() const  {
  FeatureInfo output_feature_info;
  for (int input_feature_index = 0;
       input_feature_index < (int)input_feature_info_.feature_count();
       input_feature_index++) {
    if (!isNaN(neutral_object_.features()[input_feature_index])) {
      output_feature_info.addFeature(
        input_feature_info_.getFeatureType(input_feature_index));
    }
  }
  return output_feature_info;
}

string NanToNeutralConverter::generateCppCode(
  const std::string& function_name) const {
    string code;
    code.
      append("#include <limits>\n").
      append("#include <vector>\n\nvoid ").
      append(function_name).
      append("(const std::vector<double>& features, ").
      append("std::vector<double>* result) {\n").
      append("  result->clear();\n\n").
      append("  std::vector <double> neutral_values;\n");
    for (int index = 0;
         index < (int)neutral_object_.features().size(); ++index) {
      code.append("  neutral_values.push_back(");
      if (isNaN(neutral_object_.features()[index])) {
        code.append("std::numeric_limits<double>::quiet_NaN()");
      } else {
        code.append(lexical_cast<string>(neutral_object_.features()[index]));
      }
      code.append(");\n");
    }
    code.
      append("\n").
      append("  for (int index = 0; index < features.size(); ++index) {\n").
      append("   if (neutral_values[index] == neutral_values[index]) {\n").
      append("     if (features[index] == features[index]) {\n").
      append("       result->push_back(features[index]);\n").
      append("     } else {\n").
      append("       result->push_back(neutral_values[index]);\n").
      append("     }\n").
      append("   }\n").
      append("  }\n").
      append("}\n");
    return code;
}

void NanToNeutralConverter::applyImpl(const Object& input,
                                      Object* output) const {
  Object converted_object = input.deepCopy();
  converted_object.features().clear();
  for (int feature_index = 0;
       feature_index < (int)input.features().size();
       ++feature_index) {
    if (!isNaN(neutral_object_.features()[feature_index])) {
      if (isNaN(input[feature_index])) {
        converted_object << neutral_object_.features()[feature_index];
      } else {
        converted_object << input[feature_index];
      }
    }
  }
  *output = converted_object;
}

string NanToNeutralConverter::getDefaultAlias() const {
  return "NanToNeutralConverter";
}

string NanToNeutralConverter::toString() const {
  return "NaN-to-neutral converter with object: (" + neutral_object_.toString() + ")";
}
};
