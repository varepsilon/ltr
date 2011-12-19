// Copyright 2011 Yandex

#include <string>
#include <boost/lexical_cast.hpp>

#include "ltr/scorers/one_feature_scorer.h"

using std::string;

namespace ltr {

  string OneFeatureScorer::brief() const {
    string result = "takes feature " +
      boost::lexical_cast<string>(index_);
    return result;
  }

  string OneFeatureScorer::generateCppCodeImpl
      (const string& class_name, int tabbing) const {
    string hpp_code;

    string tab_str(tabbing, '\t');

    hpp_code.
      append(tab_str).
        append("class ").
        append(class_name).
        append(" {\n").
      append(tab_str).
        append("public:\n").
      append(tab_str).
        append("\tstatic double score").
        append("(const std::vector<double>& features) { return ").
        append("features[").
        append(boost::lexical_cast<string>(index_)).
        append("]; }\n").
      append(tab_str).
        append("};\n");

    return hpp_code;
  }
};
