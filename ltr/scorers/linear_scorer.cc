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
  double output;
  for (int i = 0; i < obj.size(); ++i) {
    output += obj.features()[i] * weights(i);
  }
  return output;
}

string LinearScorer::generateCppCodeImpl(const string& function_name) const {
  string code;
  code.append("double " + function_name +
      "(const std::vector<double>& features) {\n").
    append("\tdouble output = 0.0;\n");
  for (int i = 0; i < weights.size(); ++i) {
    code.append("\toutput += features[" + lexical_cast<string>(i) +
       "] * " + lexical_cast<string>(weights(i)) + ";\n");
  }
  code.append("\treturn output;\n").
    append("}\n");
  return code;
}
};
