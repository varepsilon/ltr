// Copyright 2011 Yandex

#include <rlog/rlog.h>

#include <string>
#include <sstream>

#include "ltr/scorers/one_feature_scorer.h"

using std::string;

namespace ltr {

  string OneFeatureScorer::toStringImpl() const {
    std::stringstream str;
    str << "Scorer, which takes feature no. " << index_;
    return str.str();
  }

  string OneFeatureScorer::generateCppCodeImpl(
      const string& function_name) const {
    rInfo("Starting to generate CPP code for OneFeatureScorer");
    string hpp_code;

    hpp_code.
      append("inline double ").
      append(function_name).
      append("(const std::vector<double>& features) { return ").
      append("features[").
      append(ltr::utility::lexical_cast<string>(index_)).
      append("]; }\n");

    return hpp_code;
  }
};
