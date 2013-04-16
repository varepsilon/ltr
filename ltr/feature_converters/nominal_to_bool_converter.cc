// Copyright 2012 Yandex

#include "ltr/feature_converters/nominal_to_bool_converter.h"

#include <sstream>

#include "ltr/utility/boost/lexical_cast.h"

using std::stringstream;

using ltr::utility::lexical_cast;

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
       input_feature_index++) {
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
    << "  vector<int> feature_outputs["
      << lexical_cast<string>(input_feature_info_.feature_count()) << "];\n";

  for (int input_feature_index = 0;
       input_feature_index < input_feature_info_.feature_count();
       input_feature_index++) {
    if (input_feature_info_.getFeatureType(input_feature_index) == NOMINAL) {
      const map<int, string>& vals =
        input_feature_info_.getNominalFeatureValues(input_feature_index);
      for (map<int, string>::const_iterator iterator = vals.begin();
           iterator != vals.end(); ++iterator) {
        code
          << "  feature_outputs["
          << lexical_cast<string>(input_feature_index)
          << "].push_back("
          << lexical_cast<string>(iterator->first)
          << ");\n";
      }
    }
  }
  code
    << "  for (int i = 0; i < features.size(); ++i) {\n"
    << "    if (!nominal[i])\n"
    << "      result->push_back(features[i]);\n"
    << "  }\n"
    << "\n"
    << "  for (int i = 0; i < features.size(); ++i) {\n"
    << "    if (nominal[i]) {\n"
    << "      for (int j = 0; j < feature_outputs[i].size(); j++)\n"
    << "        result->push_back(features[i] == feature_outputs[i][j]);\n"
    << "    }\n"
    << "  }\n"
    << "}\n";
  return code.str();
}

void NominalToBoolConverter::applyImpl(
  const Object& input, Object* output) const {
    Object converted_object = input.deepCopy();
    converted_object.clear();
    for (int input_feature_index = 0;
         input_feature_index < (int)input.feature_count();
         ++input_feature_index) {
        if (input_feature_info_.getFeatureType(
            input_feature_index) != NOMINAL) {
          converted_object << input[input_feature_index];
        }
    }

    for (int input_feature_index = 0;
         input_feature_index < (int)input.feature_count();
         ++input_feature_index) {
        if (input_feature_info_.getFeatureType(
            input_feature_index) == NOMINAL) {
          const map<int, string>& vals =
            input_feature_info_.getNominalFeatureValues(input_feature_index);
          for (map<int, string>::const_iterator iterator = vals.begin();
            iterator != vals.end(); ++iterator) {
              if (input[input_feature_index] == iterator->first) {
                converted_object << 1.0;
              } else {
                converted_object << 0.0;
              }
          }
        }
    }
    converted_object.set_feature_count(input.feature_count());
    *output = converted_object;
}

string NominalToBoolConverter::getDefaultAlias() const {
  return "NominalToBoolConverter";
}

string NominalToBoolConverter::toString() const {
  return "Nominal-to-bool converter";
}
};
