// Copyright 2011 Yandex

#include "ltr/scorers/linear_scorer.h"

#include <rlog/rlog.h>
#include <string>
#include <sstream>

#include "ltr/utility/boost/lexical_cast.hpp"

using std::string;
using std::stringstream;

using ltr::utility::lexical_cast;

namespace ltr {

string LinearScorer::toStringImpl() const {
  stringstream str;
  str << "Linear scorer with coefficients: ";
  str << vectorToString(weight_);
  return str.str();
}

double LinearScorer::scoreImpl(const Object& object) const {
  double output = weight_[0];
  for (int i = 0; i < (int)object.feature_count(); ++i) {
    output += object[i] * weight_[i + 1];
  }
  return output;
}

string LinearScorer::generateCppCodeImpl(const string& function_name) const {
  stringstream code;
  code
    << "double " + function_name + "(const std::vector<double>& features) {\n"
    << "  double output = " << lexical_cast<string>(weight_[0]) << ";\n";
  for (int i = 1; i < (int)weight_.size(); ++i) {
    code
      << "  output += " << lexical_cast<string>(weight_[i])
        << " * features[" << lexical_cast<string>(i - 1) << "];\n";
  }
  code
    << "  return output;\n"
    << "}\n";
  return code.str();
}
};
