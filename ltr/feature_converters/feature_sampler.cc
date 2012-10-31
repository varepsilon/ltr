// Copyright 2012 Yandex

#include <boost/lexical_cast.hpp>

#include <string>
#include <stdexcept>

#include "ltr/feature_converters/feature_sampler.h"

using std::logic_error;
using std::string;
using std::stringstream;

namespace ltr {
FeatureInfo FeatureSampler::convertFeatureInfo() const {
  FeatureInfo output_feature_info(indices_.size());
  for (int i = 0; i < (int)indices_.size(); ++i) {
    CHECK(indices_[i] < input_feature_info_.feature_count());
    FeatureType feature_type =
        input_feature_info_.getFeatureType(indices_[i]);
    output_feature_info.setFeatureType(i, feature_type);
  }
  return output_feature_info;
}

string FeatureSampler::generateCppCode(const string& function_name) const {
    string code;
    code.
      append("#include <vector>\n\nvoid ").
      append(function_name).
      append("(const std::vector<double>& features, ").
      append("std::vector<double>* result) {\n").
      append("  result->clear();\n").
      append("  int indices[] = {");
    for (int i = 0; i < (int)indices_.size(); ++i) {
      if (i != 0)
        code.append(",");
      code.append(boost::lexical_cast<string>(indices_[i]));
    }
    code.
      append("};\n").
      append("  for (int i = 0; i < ").
      append(boost::lexical_cast<string>(indices_.size())).
      append("; ++i) {\n").
      append("    result->push_back(features[indices[i]]);\n").
      append("  }\n").
      append("}\n");
    return code;
}

void FeatureSampler::set_indices(const Indices& indices) {
  for (int index = 0; index < (int) indices.size(); ++index) {
    CHECK(0 <= indices[index] &&
      indices[index] < input_feature_info_.feature_count());
  }
  indices_ = indices;
}

void FeatureSampler::applyImpl(const Object& input, Object* output) const {
  Object converted_object = input.deepCopy();
  converted_object.features().resize(indices_.size());
  for (int index = 0; index < (int)indices_.size(); ++index) {
    converted_object[index] = input[indices_[index]];
  }
  *output = converted_object;
}

string FeatureSampler::getDefaultAlias() const {
  return "FeatureSampler";
}

string FeatureSampler::toString() const {
  stringstream str;
  str << "Feature sampler with indices: ";
  str << vectorToString(indices_);
  return str.str();
}
};
