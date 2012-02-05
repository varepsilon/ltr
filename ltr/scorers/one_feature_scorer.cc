// Copyright 2011 Yandex

#include <boost/lexical_cast.hpp>
#include <string>

#include "ltr/scorers/one_feature_scorer.h"

using std::string;

namespace ltr {

  string OneFeatureScorer::brief() const {
    string result = "takes feature " +
      boost::lexical_cast<string>(index_);
    return result;
  }

  string OneFeatureScorer::generateCppCode(const string& function_name) const {
    string hpp_code;

    hpp_code.
      append("inline double ").
      append(function_name).
      append("(const std::vector<double>& features) { return ").
      append("features[").
      append(boost::lexical_cast<string>(index_)).
      append("]; }\n");

    return hpp_code;
  }
};
