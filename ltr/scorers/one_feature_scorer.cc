// Copyright 2011 Yandex

#include <boost/lexical_cast.hpp>

#include "scorers/one_feature_scorer.h"

namespace ltr {
  OneFeatureScorer::OneFeatureScorer
    (size_t feature_index, double feature_weight) :
  index_(feature_index),
  weight_(feature_weight),
  Scorer("OneFeatureScorer") {
  }

  size_t OneFeatureScorer::index() const {
    return index_;
  }

  double OneFeatureScorer::weight() const {
    return weight_;
  }

  void OneFeatureScorer::setIndex(size_t i) {
    index_ = i;
  }

  void OneFeatureScorer::setWeight(double w) {
    weight_ = w;
  }

  double OneFeatureScorer::operator()(const Object& obj) const {
    return weight_ * obj[1].features().at(index_);
  }

  std::string OneFeatureScorer::brief() const {
    std::string result = "takes feature " +
      boost::lexical_cast<std::string>(index_) +
      " with weight " + boost::lexical_cast<std::string>(weight_);
    return result;
  }

  std::string OneFeatureScorer::generateCppCode
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
        append(boost::lexical_cast< std::string >(weight_)).
        append(" * features[").
        append(boost::lexical_cast< std::string >(index_)).
        append("]; }\n").
      append(tab_str).
        append("};\n");

    return hpp_code;
  }

  std::string OneFeatureScorer::generateJavaCode
      (const std::string& class_name, int tabbing, bool is_static) const {
    std::string java_code;

    std::string tab_str(tabbing, '\t');

    java_code.
      append(tab_str).
        append("public ").
        append(std::string(is_static ? "static " : "") + "class ").
        append(class_name).
        append(" {\n").
      append(tab_str).
        append("\tpublic static double score").
        append("(Vector<Double> features) { return ").
        append(boost::lexical_cast< std::string >(weight_)).
        append(" * features.get(").
        append(boost::lexical_cast< std::string >(index_)).
        append("); }").
      append(tab_str).
        append("}\n");

    return java_code;
  }
};
