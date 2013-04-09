// Copyright 2011 Yandex

#include "ltr/feature_converters/per_feature_linear_converter.h"

#include <stdexcept>
#include <sstream>

#include "ltr/utility/boost/lexical_cast.h"

using std::logic_error;
using std::stringstream;

using ltr::utility::lexical_cast;

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
  stringstream code;
  code
    << "#include <vector>\n"
    << "\n"
    << "void " << function_name << "(const std::vector<double>& features,\n"
    << "    std::vector<double>* result) {\n"
    << "  result->clear();\n";
    for (int i = 0; i < (int)factor_.size(); ++i) {
      code
        << "  result->push_back(features[" << lexical_cast<string>(i) << "] * "
        << lexical_cast<string>(factor_[i]) << " + "
        << lexical_cast<string>(shift_[i]) << ");\n";
    }
  code << "}\n";
  return code.str();
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
