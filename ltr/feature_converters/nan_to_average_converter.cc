// Copyright 2012 Yandex

#include "boost/lexical_cast.hpp"

#include "ltr/feature_converters/nan_to_average_converter.h"

namespace ltr {
void NanToAverageConverter::fillOutputFeatureInfo()  {
  // TO DO (not very good)
  if (average_features_values_.empty()) {
    return;
  }

  output_feature_info_.clear();
  for (int input_feature_index = 0;
       input_feature_index < (int)input_feature_info_.feature_count();
       input_feature_index++) {
    if (!isNaN(average_features_values_[input_feature_index])) {
      output_feature_info_.addFeature(
        input_feature_info_.getFeatureType(input_feature_index));
    }
  }
}

string NanToAverageConverter::generateCppCode(
  const std::string& function_name) const {
    string code;
    code.
      append("#include <limits>\n").
      append("#include <vector>\n\nvoid ").
      append(function_name).
      append("(const std::vector<double>& features, ").
      append("std::vector<double>* result) {\n").
      append("  result->clear();\n\n").
      append("  std::vector <double> average_values;\n");
    for (int i = 0; i < (int)average_features_values_.size(); ++i) {
      code.append("  average_values.push_back(");
      if (isNaN(average_features_values_[i])) {
        code.append("std::numeric_limits<double>::quiet_NaN()");
      } else {
        code.append(lexical_cast<string>(average_features_values_[i]));
      }
      code.append(");\n");
    }
    code.
      append("\n").
      append("  for (int i = 0; i < features.size(); ++i) {\n").
      append("   if (average_values[i] == average_values[i]) {\n").
      append("     if (features[i] == features[i]) {\n").
      append("       result->push_back(features[i]);\n").
      append("     } else {\n").
      append("       result->push_back(average_values[i]);\n").
      append("     }\n").
      append("   }\n").
      append("  }\n").
      append("}\n");
    return code;
}

void NanToAverageConverter::applyImpl(const Object& input,
                                      Object* output) const {
  *output = Object(output_feature_info_);
  output->features().resize(output_feature_info_.feature_count());

  int input_feature_index = 0, output_feature_index = 0;
  for (; input_feature_index < (int)input.features().size();
       ++input_feature_index) {
    if (!isNaN(average_features_values_[input_feature_index])) {
      if (isNaN(input[input_feature_index])) {
        output->at(output_feature_index) =
          average_features_values_[input_feature_index];
      } else {
        output->at(output_feature_index) = input[input_feature_index];
      }
      ++output_feature_index;
    }
  }
}

string NanToAverageConverter::getDefaultAlias() const {
  return "NanToAverageConverter";
}
};
