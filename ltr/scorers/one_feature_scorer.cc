// Copyright 2011 Yandex

#include <boost/lexical_cast.hpp>

#include "scorers/one_feature_scorer.h"

namespace ltr {

  std::string OneFeatureScorer::brief() const {
    std::string result = "takes feature " +
      boost::lexical_cast<std::string>(index_);
    return result;
  }

  std::string OneFeatureScorer::generateCppCodeImpl
      (const std::string& class_name, int tabbing) const {
    std::string hpp_code;

    std::string tab_str(tabbing, '\t');

    hpp_code.
      append(tab_str).
        append("class ").
        append(class_name).
        append(" {\n").
      append(tab_str).
        append("public:\n").
      append(tab_str).
        append("\tstatic double score").
        append("(const std::vector< double >& features) { return ").
        append("features[").
        append(boost::lexical_cast< std::string >(index_)).
        append("]; }\n").
      append(tab_str).
        append("};\n");

    return hpp_code;
  }
};
