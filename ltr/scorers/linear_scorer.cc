// Copyright 2011 Yandex

#include "ltr/scorers/linear_scorer.h"

#include "ltr/utility/boost/lexical_cast.h"
#include <rlog/rlog.h>
#include <string>
#include <sstream>

using std::string;
using ltr::utility::lexical_cast;
using std::stringstream;

namespace ltr {

string LinearScorer::toStringImpl() const {
  stringstream str;
  str << "Linear scorer with coefficients: ";
  str << vectorToString(weight_);
  return str.str();
}

double LinearScorer::scoreImpl(const Object& object) const {
  double output = weight_[0];
  for (int i = 0; i < (int)object.features().size(); ++i) {
    output += object[i] * weight_[i + 1];
  }
  return output;
}

string LinearScorer::generateCppCodeImpl(const string& function_name) const {
  string code;
  code.append(
    "double " + function_name + "(const std::vector<double>& features) {\n")
      .append(
    "  double output = " + lexical_cast<string>(weight_[0]) + ";\n");
  for (int i = 1; i < (int)weight_.size(); ++i) {
    code.append(
    "  output += " + lexical_cast<string>(weight_[i]) +
             " * features[" + lexical_cast<string>(i - 1) +"];\n");
  }
  code.append("  return output;\n")
      .append("}\n");
  return code;
}
};
