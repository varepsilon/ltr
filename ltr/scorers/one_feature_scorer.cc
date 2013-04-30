// Copyright 2011 Yandex

#include "ltr/scorers/one_feature_scorer.h"

#include <rlog/rlog.h>

#include <string>
#include <sstream>

#include "ltr/utility/boost/lexical_cast.hpp"

using std::string;
using std::stringstream;
using ltr::utility::lexical_cast;

namespace ltr {

  string OneFeatureScorer::toStringImpl() const {
    std::stringstream str;
    str << "Scorer, which takes feature no. " << index_;
    return str.str();
  }

  string OneFeatureScorer::generateCppCodeImpl(
      const string& function_name) const {
    rInfo("Starting to generate CPP code for OneFeatureScorer");
    stringstream code;

    code
      << "inline double " << function_name
        << "(const std::vector<double>& features) {\n"
      << "  return features[" << lexical_cast<string>(index_) << "];\n"
      << "}\n";

    return code.str();
  }
};
