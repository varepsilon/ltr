// Copyright 2011 Yandex

#include "scorers/soft_scorer.h"
#include <string>
#include <boost/lexical_cast.hpp>

namespace ltr {
  LinearScorer::LinearScorer(const Gradient& parameters,
      const FeatureConverterArray& featureConverters) :
    SoftScorer("LinearScorer", featureConverters),
    features_weights_(parameters) {
  }

  std::string LinearScorer::brief() const {
    return std::string("Linear Ranker - use vector of feature").
      append(" weights and calculate result via <w, f>");
  }

  double LinearScorer::scoreImpl(const Object& object) const {
    double scrore = 0;
    for (int feature = 0; feature < features_weights_.size(); ++feature) {
      scrore += features_weights_[feature] * object.features()[feature];
    }
    return scrore;  
  }

  Gradient LinearScorer::getGradient(const Object& obj) const {
    Gradient output(features_weights_);
    for (int feature = 0; feature < features_weights_.size(); ++feature) {
      output[feature] = obj.features()[feature];
    }
    return output;
  }

  size_t LinearScorer::getParametersCount() const {
    return features_weights_.size();
  }

  double LinearScorer::getParameter(int parameter_index) const {
    return features_weights_[parameter_index];
  }

  void LinearScorer::shiftParameters(const Gradient& shift) {
    for (int feature = 0; feature < features_weights_.size(); ++feature) {
      features_weights_[feature] += shift[feature];
    }
  }

  std::string LinearScorer::generateCppCode(const std::string& class_name,
      int tabbing) const {
    std::string hpp_code;
    std::string tab_str(tabbing, '\t');

    hpp_code.
      append("class ").
      append(class_name).
      append(" {\n").
      append("public:\n").
      append("  static double score(const std::vector< double >& features) {\n").
      append("    double result = 0;\n");

    for(int feature = 0; feature < features_weights_.size(); ++feature) {
      hpp_code.
        append("    result += ").
        append(boost::lexical_cast<std::string>(features_weights_[feature])).
        append(" * features[").
        append(boost::lexical_cast<std::string>(feature)).
        append("];\n");
    }

    hpp_code.
      append("    return result;\n").
      append("  }\n").
    append("};\n");

    return hpp_code;
  }

  std::string LinearScorer::generateJavaCode(const std::string& class_name,
      int tabbing, bool is_static) const {
    std::string java_code;
    std::string tab_str(tabbing, '\t');

    java_code.
      append("public ").
      append(std::string(is_static ? "static " : "" ) + "class ").
      append(class_name).
      append(" {\n").
      append("\tpublic static double score(Vector<Double> features) { \n").
      append("    double result = 0;\n");

    for(int feature = 0; feature < features_weights_.size(); ++feature) {
      java_code.
        append("    result += ").
        append(boost::lexical_cast<std::string>(features_weights_[feature])).
        append(" * features.get(").
        append(boost::lexical_cast<std::string>(feature)).
        append(");\n");
    }

    java_code.
      append("    return result;\n").
      append("  }\n").
    append("}\n");

    return java_code;
  }
};
