// Copyright 2012 Yandex

#include "ltr/feature_converters/nan_to_neutral_converter.h"

#include <sstream>

using std::stringstream;

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
    stringstream code;
    code
      << "#include <limits>\n"
      << "#include <vector>\n"
      << "\n"
      << "void " << function_name << "(const std::vector<double>& features,\n"
      << "    std::vector<double>* result) {\n"
      << "  result->clear();\n\n"
      << "  std::vector <double> neutral_values;\n";
    for (int index = 0;
         index < (int)neutral_object_.features().size(); ++index) {
      code << "  neutral_values.push_back(";
      if (isNaN(neutral_object_.features()[index])) {
        code << "std::numeric_limits<double>::quiet_NaN()";
      } else {
        code << lexical_cast<string>(neutral_object_.features()[index]);
      }
      code << ");\n";
    }
    code
      << "\n"
      << "  for (int index = 0; index < features.size(); ++index) {\n"
      << "   if (neutral_values[index] == neutral_values[index]) {\n"
      << "     if (features[index] == features[index]) {\n"
      << "       result->push_back(features[index]);\n"
      << "     } else {\n"
      << "       result->push_back(neutral_values[index]);\n"
      << "     }\n"
      << "   }\n"
      << "  }\n"
      << "}\n";
    return code.str();
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
  return "NaN-to-neutral converter with object: ("
    + neutral_object_.toString() + ")";
}
};
