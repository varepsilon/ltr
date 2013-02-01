// Copyright 2012 Yandex

#include "ltr/scorers/bayesian_scorer.h"

#include <cmath>

using ltr::BayesianScorer;

namespace ltr {
double BayesianScorer::scoreImpl(const Object& object) const {
  double result = 0;
  double max_score = 0;
  for (LabelToPriorProbability::const_iterator label_iterator =
        prior_probability_.begin();
       label_iterator != prior_probability_.end();
       ++label_iterator) {
    double label = label_iterator->first;
    double probability = label_iterator->second;
    double current_score = std::log(probability) +
      estimator_->estimate(object, label);
    if (current_score > max_score ||
        label_iterator == prior_probability_.begin()) {
      max_score = current_score;
      result = label_iterator->first;
    }
  }
  return result;
}

string BayesianScorer::generateCppCodeImpl(const string& function_name) const {
  string result;

  string estimator_name = function_name + "_estimator";
  result += estimator_->generateCppCode(estimator_name);

  result += "double " + function_name +
    "(const std::vector<double>& features) {\n";
  result += "  Object object;\n";
  result += "  for (int index = 0; index < features.size(); ++index) {\n";
  result += "  object << features[index];\n";
  result += " }\n";

  result += " double res = 0;\n";
  result += " double max_score = 0;\n";

  result += " map<double, double> prior;\n";
  for (LabelToPriorProbability::const_iterator label_iterator =
        prior_probability_.begin();
       label_iterator != prior_probability_.end();
       ++label_iterator) {
    result += " prior[" +
              lexical_cast<string>(label_iterator->first) +
              "] = " +
              lexical_cast<string>(label_iterator->second) +
              ";\n";
  }

  result += " for (map<double, double>::const_iterator it =\n";
  result += "       prior.begin();\n";
  result += "      it != prior.end();\n";
  result += "      ++it) {\n";
  result += "   double label = it->first;\n";
  result += "   double probability = it->second;\n";
  result += "   double current_score = log(probability) +\n";
  result +=      estimator_name + "(object, label);\n";
  result += "   if (current_score > max_score ||\n";
  result += "       it == prior.begin()) {\n";
  result += "     max_score = current_score;\n";
  result += "     res = it->first;\n";
  result += "   }\n";
  result += " }\n";
  result += " return res;\n";
  result += " }\n";
  return result;
};
}
