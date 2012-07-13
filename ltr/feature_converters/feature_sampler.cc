// Copyright 2011 Yandex

#include <boost/lexical_cast.hpp>

#include <string>
#include <stdexcept>
#include "ltr/feature_converters/feature_sampler.h"

using std::logic_error;
using std::string;

namespace ltr {

void FeatureSampler::set_indices(const Indices& indices) {
  indices_ = indices;
  fillOutputFeatureInfo();
  // \TODO(sameg) Discuss, how many  fillOutputFeatureInfo calls we need.
}

void FeatureSampler::fillOutputFeatureInfo() {
  if (input_feature_info_.empty()) {
    return;
  }
  output_feature_info_.resize(indices_.size());
  for (int i = 0; i < indices_.size(); ++i) {
    CHECK(indices_[i] < input_feature_info_.feature_count());
    FeatureType feature_type =
        input_feature_info_.getFeatureType(indices_[i]);
    output_feature_info_.setFeatureType(i, feature_type);
  }
}

void FeatureSampler::applyImpl(const Object& input, Object* output) const {
  output->setFeatureInfo(output_feature_info_);
  for (int i = 0; i < indices_.size(); ++i) {
    output->features()[i] = input.features()[indices_[i]];
  }
  // \FIXME(sameg): Extra copy of output_feature_info.
  // Do we really need to have feature_info in object??
  // In this case output_feature_info_ must became a FeatureInfo::Ptr
  // output->set_feature_info(output_feature_info_);
}

string FeatureSampler::generateCppCode(const string& function_name) const {
    string code;
    code.
      append("#include <vector>\n\nvoid ").
      append(function_name).
      append("(const std::vector<double>& features, ").
      append("std::vector<double>* result) {\n").
      append("  result->clear();\n").
      append("  size_t indices[] = {");
    for (size_t i = 0; i < indices_.size(); i++) {
      if (i != 0)
        code.append(",");
      code.append(boost::lexical_cast<string>(indices_[i]));
    }
    code.
      append("};\n").
      append("  for (size_t i = 0; i < ").
      append(boost::lexical_cast<string>(indices_.size())).
      append("; i++) {\n").
      append("    result->push_back(features[indices[i]]);\n").
      append("  }\n").
      append("}\n");
    return code;
}
}
