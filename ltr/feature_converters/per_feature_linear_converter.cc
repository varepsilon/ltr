// Copyright 2011 Yandex

#include <stdexcept>

#include "ltr/feature_converters/per_feature_linear_converter.h"

using std::logic_error;
using std::stringstream;

namespace ltr {
FeatureInfo PerFeatureLinearConverter::convertFeatureInfo() const {
  return input_feature_info_;
}

void PerFeatureLinearConverter::resize(const FeatureInfo& input_feature_info) {
  factor_.resize(input_feature_info.feature_count(), 1.);
  shift_.resize(input_feature_info.feature_count(), 0.);
}

string PerFeatureLinearConverter::generateCppCode(
    const string& function_name) const {
  string code;
  code.
    append("#include <vector>\n\nvoid ").
    append(function_name).
    append("(const std::vector<double>& features, ").
    append("std::vector<double>* result) {\n").
    append("  result->clear();\n");
    for (int i = 0; i < (int)factor_.size(); ++i) {
      code.
        append("  result->push_back(features[").
        append(boost::lexical_cast<string>(i)).
        append("] * ").
        append(boost::lexical_cast<string>(factor_[i])).
        append(" + ").
        append(boost::lexical_cast<string>(shift_[i])).
        append(");\n");
    }
  code.append("}\n");
  return code;
}

void PerFeatureLinearConverter::applyImpl(const Object& input,
                                                Object* output) const {
  Object converted_object = input.deepCopy();
  for (int feature_index = 0;
       feature_index < (int)input.features().size(); ++feature_index) {
    converted_object[feature_index] *= factor_[feature_index];
    converted_object[feature_index] += shift_[feature_index];
  }
  *output = converted_object;
}

string PerFeatureLinearConverter::getDefaultAlias() const {
  return "PerFeatureLinearConverter";
}

string PerFeatureLinearConverter::toString() const {
  stringstream str;
  str << "Linear feature converter with factors: ";
  str << vectorToString(factor_);
  str << " and shifts ";
  str << vectorToString(shift_);
  return str.str();
}
};
