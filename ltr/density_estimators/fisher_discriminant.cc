// Copyright 2012 Yandex
#include <cmath>

#include "ltr/density_estimators/fisher_discriminant.h"
#include "ltr/utility/eigen_converters.h"

using ltr::FisherDiscriminant;
using ltr::utility::InitEigenVector;

namespace ltr {
  double FisherDiscriminant::estimate(const Object& object,
                                      const double label) {
    VectorXd features(object.feature_count());
    InitEigenVector(&features);

    for (int feature_index = 0;
         feature_index < object.feature_count();
         ++feature_index) {
      features(feature_index) = object[feature_index];
    }

    return features.dot(linear_factors_[label]) + linear_biases_[label];
  }

  string FisherDiscriminant::generateCppCode(
      const string& function_name) const {
    string result;
    result += "#include <Eigen/Dense>\n";
    result += "#include <map>\n";

    result += "using Eigen::VectorXd;\n";
    result += "using std::map;\n";

    result += "double " + function_name +
    "(const Object& object, const double label) {\n";
    result += " VectorXd features(object.feature_count());\n";

    result += " for (int element_index = 0;\n";
    result += "   element_index < features.size();\n";
    result += "   ++element_index) {\n";
    result += "  features(element_index) = object[element_index];\n";
    result += " }\n";

    result += " map<double, VectorXd> factors;\n";
    result += "int sz;\n";
    result += "double lab;\n";
    for (map<double, VectorXd>::const_iterator it = linear_factors_.begin();
         it != linear_factors_.end();
         ++it) {
      result += "lab = " + lexical_cast<string>(it->first) + ";\n";
      result += "sz = " + lexical_cast<string>(it->second.size()) + ";\n";
      result += "factors[lab] = VectorXd(sz);\n";
      for (int index = 0; index < it->second.size(); ++index) {
        result += "factors[lab](" +
                  lexical_cast<string>(index) +
                  ") = " +
                  lexical_cast<string>(it->second(index)) +
                  ";\n";
      }
      result += "\n";
    }

    result += "map<double, double> biases;\n";
    for (map<double, double>::const_iterator it = linear_biases_.begin();
         it != linear_biases_.end();
         ++it) {
      result += "biases[" +
                lexical_cast<string>(it->first) +
                "] = " +
                lexical_cast<string>(it->second) +
                ";\n";
    }

    result += " for (int feature_index = 0;\n";
    result += "      feature_index < object.feature_count();\n";
    result += "      ++feature_index) {\n";
    result += "   features(feature_index) = object[feature_index];\n";
    result += " }\n";

    result += "return features.dot(factors[label]) + biases[label];\n";
    result += "}\n\n";
    return result;
  }
};
