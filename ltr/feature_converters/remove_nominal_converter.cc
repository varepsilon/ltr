// Copyright 2012 Yandex

#include "ltr/feature_converters/remove_nominal_converter.h"

#include <sstream>

#include "ltr/utility/numerical.h"

using std::stringstream;

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
  stringstream code;
  code
    << "#include <vector>\n"
    << "\n"
    << "void " << function_name << "(const std::vector<double>& features,\n"
    << "    std::vector<double>* result) {\n"
    << "  result->clear();\n"
    << "  bool nominal[] = {";
  for (int input_feature_index = 0;
       input_feature_index < input_feature_info_.feature_count();
       ++input_feature_index) {
    if (input_feature_index != 0) {
      code << ", ";
    }
    if (input_feature_info_.getFeatureType(input_feature_index) == NOMINAL) {
      code << "1";
    } else {
      code << "0";
    }
  }
  code
    << "  };\n"
    << "  for (int i = 0; i < features.size(); ++i) {\n"
    << "    if (!nominal[i])\n"
    << "      result->push_back(features[i]);\n"
    << "  }\n"
    << "}\n";
  return code.str();
}

void RemoveNominalConverter::applyImpl(const Object& input,
                                             Object* output) const {
  Object converted_object = input.deepCopy();
  converted_object.clear();
  for (int input_feature_index = 0;
       input_feature_index < (int)input.feature_count();
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

string RemoveNominalConverter::toString() const {
  return "Remove-nominal converter";
}
};
