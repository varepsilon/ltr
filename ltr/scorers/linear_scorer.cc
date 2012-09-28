// Copyright 2011 Yandex

#include "ltr/scorers/linear_scorer.h"

#include <boost/lexical_cast.hpp>
#include <logog/logog.h>
#include <string>
#include <sstream>

using std::string;
using boost::lexical_cast;

namespace ltr {
string LinearScorer::toString() const {
  std::stringstream str;
  std::fixed(str);
  str.precision(2);
  str << "Linear scorer with coefficients: [";
  for (size_t i = 0; i < weights.size(); ++i) {
    if (i != 0) {
      str << ", ";
    }
    str << weights[i];
  }
  str << "]";
  return str.str();
}

double LinearScorer::scoreImpl(const Object& object) const {
  double output = weights[0];
  for (int i = 0; i < (int)object.features().size(); ++i) {
    output += object[i] * weights[i + 1];
  }
  return output;
}

string LinearScorer::generateCppCodeImpl(const string& function_name) const {
  string code;
  code.append("double " + function_name +
      "(const std::vector<double>& features) {\n").
    append("\tdouble output = " + lexical_cast<string>(weights[0]) + ";\n");
  for (int i = 1; i < (int)weights.size(); ++i) {
    code.append("\toutput += " + lexical_cast<string>(weights[i]) +
      " * features[" + lexical_cast<string>(i - 1) +"];\n");
  }
  code.append("\treturn output;\n").
    append("}\n");
  return code;
}
};
