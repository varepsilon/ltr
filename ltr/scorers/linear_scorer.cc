// Copyright 2011 Yandex

#include "ltr/scorers/linear_scorer.h"

#include <boost/lexical_cast.hpp>
#include <string>

using std::string;
using boost::lexical_cast;

namespace ltr {
string LinearScorer::brief() const {
  return string("Linear scorer - uses vector of feature's").
    append(" weights and calculate result via <w, f>");
}

double LinearScorer::scoreImpl(const Object& obj) const {
  double output = weights[0];
  for (int i = 0; i < obj.features().size(); ++i) {
    output += obj.features()[i] * weights[i + 1];
  }
  return output;
}

string LinearScorer::generateCppCodeImpl(const string& function_name) const {
  string code;
  code.append("double " + function_name +
      "(const std::vector<double>& features) {\n").
    append("\tdouble output = " + lexical_cast<string>(weights[0]) + ";\n");
  for (int i = 1; i < weights.size(); ++i) {
    code.append("\toutput += " + lexical_cast<string>(weights[i]) +
      " * features[" + lexical_cast<string>(i - 1) +"];\n");
  }
  code.append("\treturn output;\n").
    append("}\n");
  return code;
}
};
