// Copyright 2012 Yandex

#include "ltr/feature_converters/feature_sampler.h"

#include <string>
#include <sstream>
#include <stdexcept>

#include "ltr/utility/boost/lexical_cast.h"

using std::logic_error;
using std::string;
using std::stringstream;

using ltr::utility::lexical_cast;

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
    stringstream code;
    code
      << "#include <vector>\n"
      << "\n"
      << "void " << function_name << "(const std::vector<double>& features,\n"
      << "    std::vector<double>* result) {\n"
      << "  result->clear();\n"
      << "  int indices[] = {";
    for (int i = 0; i < (int)indices_.size(); ++i) {
      if (i != 0)
        code << ", ";
      code << lexical_cast<string>(indices_[i]);
    }
    code
      << "};\n"
      << "  for (int i = 0; i < " << lexical_cast<string>(indices_.size())
        << "; ++i) {\n"
      << "    result->push_back(features[indices[i]]);\n"
      << "  }\n"
      << "}\n";
    return code.str();
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
