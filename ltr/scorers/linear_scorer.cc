// Copyright 2011 Yandex

#include "ltr/scorers/linear_scorer.h"

#include <boost/lexical_cast.hpp>
#include <rlog/rlog.h>
#include <string>
#include <sstream>

using std::string;
using boost::lexical_cast;
using std::stringstream;

namespace ltr {

double LinearScorer::weight(int index) const {
  return weights_[index];
}

void LinearScorer::set_weight(int index, double weight) {
  weights_[index] = weight;
}

int LinearScorer::weights_count() const {
  return weights_.size();
}

string LinearScorer::toStringImpl() const {
  stringstream str;
  str << "Linear scorer with coefficients: ";
  str << vectorToString(weights_);
  return str.str();
}

double LinearScorer::scoreImpl(const Object& object) const {
  double output = weights_[0];
  for (int i = 0; i < (int)object.features().size(); ++i) {
    output += object[i] * weights_[i + 1];
  }
  return output;
}

string LinearScorer::generateCppCodeImpl(const string& function_name) const {
  string code;
  code.append(
    "double " + function_name + "(const std::vector<double>& features) {\n")
      .append(
    "  double output = " + lexical_cast<string>(weights_[0]) + ";\n");
  for (int i = 1; i < (int)weights_.size(); ++i) {
    code.append(
    "  output += " + lexical_cast<string>(weights_[i]) +
             " * features[" + lexical_cast<string>(i - 1) +"];\n");
  }
  code.append("  return output;\n")
      .append("}\n");
  return code;
}
};
