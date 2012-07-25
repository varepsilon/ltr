// Copyright 2011 Yandex

#include <stdexcept>

#include "ltr/feature_converters/per_feature_linear_converter.h"

using std::logic_error;

namespace ltr {
FeatureInfo PerFeatureLinearConverter::convertFeatureInfo() const {
  return input_feature_info_;
}

void PerFeatureLinearConverter::resize(const FeatureInfo& input_feature_info) {
  factors_.resize(input_feature_info.feature_count(), 1.);
  shifts_.resize(input_feature_info.feature_count(), 0.);
}

double PerFeatureLinearConverter::factor(int feature_index) const {
  return factors_[feature_index];
}

void PerFeatureLinearConverter::set_factor(int feature_index,
                                           double coefficient) {
  factors_[feature_index] = coefficient;
}

double PerFeatureLinearConverter::shift(int feature_index) const {
  return shifts_[feature_index];
}

void PerFeatureLinearConverter::set_shift(int feature_index, double shift) {
  shifts_[feature_index] = shift;
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
    for (int i = 0; i < (int)factors_.size(); ++i) {
      code.
        append("  result->push_back(features[").
        append(boost::lexical_cast<string>(i)).
        append("] * ").
        append(boost::lexical_cast<string>(factors_[i])).
        append(" + ").
        append(boost::lexical_cast<string>(shifts_[i])).
        append(");\n");
    }
  code.append("}\n");
  return code;
}

void PerFeatureLinearConverter::applyImpl(const Object& input,
                                                Object* output) const {
  *output = input.deepCopy();
#pragma omp parallel for
  for (int feature_index = 0;
       feature_index < (int)output->features().size(); ++feature_index) {
    output->at(feature_index) *= factors_[feature_index];
    output->at(feature_index) += shifts_[feature_index];
  }
}

string PerFeatureLinearConverter::getDefaultAlias() const {
  return "PerFeatureLinearConverter";
}
};
