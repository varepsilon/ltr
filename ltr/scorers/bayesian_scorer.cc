// Copyright 2012 Yandex

#include "ltr/scorers/bayesian_scorer.h"

#include <cmath>
#include <sstream>

using ltr::BayesianScorer;

using std::stringstream;

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
  stringstream code;
  string estimator_name = function_name + "_estimator";

  code
    << estimator_->generateCppCode(estimator_name)
    << "double " + function_name + "(const std::vector<double>& features) {\n"
    << "  Object object;\n"
    << "  for (int index = 0; index < features.size(); ++index) {\n"
    << "    object << features[index];\n"
    << "  }\n"
    << "  double res = 0;\n"
    << "  double max_score = 0;\n"
    << "  map<double, double> prior;\n";
  for (LabelToPriorProbability::const_iterator label_iterator =
         prior_probability_.begin();
       label_iterator != prior_probability_.end();
       ++label_iterator) {
    code
      << "  prior[" << lexical_cast<string>(label_iterator->first) << "] = "
        << lexical_cast<string>(label_iterator->second) << ";\n";
  }

  code
    << "  for (map<double, double>::const_iterator it = prior.begin();\n"
    << "       it != prior.end();\n"
    << "       ++it) {\n"
    << "    double label = it->first;\n"
    << "    double probability = it->second;\n"
    << "    double current_score = log(probability) +\n"
    << "      " << estimator_name << "(object, label);\n"
    << "    if (current_score > max_score || it == prior.begin()) {\n"
    << "      max_score = current_score;\n"
    << "      res = it->first;\n"
    << "    }\n"
    << "  }\n"
    << "  return res;\n"
    << "}\n";
  return code.str();
};
}
