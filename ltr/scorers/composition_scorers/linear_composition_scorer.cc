// Copyright 2011 Yandex

#include "ltr/scorers/composition_scorers/linear_composition_scorer.h"

namespace ltr {
  double LinearCompositionScorer::scoreImpl(const Object& object) const {
    double result = 0.0;
    for (size_t i = 0; i < size(); ++i) {
      result += at(i).weight * ((*at(i).scorer)(object));
    }
    return result;
  }

  string LinearCompositionScorer::generateCppCodeImpl
      (const string& function_name) const {
    string hpp_code;

    for (size_t i = 0; i < size(); ++i)
      hpp_code.append(at(i).scorer->generateCppCode());

    hpp_code.
      append("inline double ").
      append(function_name).
      append("(const std::vector< double >& features) {\n").
      append("\tdouble result = 0.0;\n");

    for (size_t i = 0; i < size(); ++i) {
      hpp_code.
        append("\tresult += ").
        append(boost::lexical_cast< string >(at(i).weight)).
        append(" * ").
        append(at(i).scorer->getDefaultSerializableObjectName()).
        append("(features);\n");
    }

    hpp_code.
      append("\treturn result;\n").
      append("};\n");

    return hpp_code;
  }

  string LinearCompositionScorer::toString() const {
    std::stringstream str;
    std::fixed(str);
    str.precision(2);
    str << "Linear composition of " << size() << " scorers: {\n";
    for (int i = 0; i < size(); ++i) {
      str << at(i).scorer->toString();
      str << " with weight " << at(i).weight << "\n";
    }
    str << "}";
    return str.str();
  }

  const vector<double>& LinearCompositionScorer::gradient
      (const vector<double>& weak_scores) const {
    vector<double> output(size());
    for (size_t i = 0; i < size(); ++i) {
      output[i] = at(i).weight;
    }
    return output;
  }
};
