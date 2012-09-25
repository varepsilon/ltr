// Copyright 2012 Yandex
#include <cmath>

#include "ltr/density_estimators/non_linear_discriminant.h"
#include "ltr/utility/eigen_converters.h"

using ltr::NonLinearDiscriminant;
using ltr::utility::InitEigenMatrix;
using ltr::utility::InitEigenVector;

namespace ltr {
  double NonLinearDiscriminant::estimate(const Object& object,
                                         const double label) {
    VectorXd features(object.feature_count());
    InitEigenVector(&features);
    for (int feature_index = 0;
         feature_index < object.feature_count();
         ++feature_index) {
      features(feature_index) = object[feature_index];
    }

    int feature_count = object.feature_count();
    double result = log(pow(M_PI, - feature_count / 2.0) *
                        pow(covariance_matrix_[label].determinant(), -0.5));
    result -= 0.5 * (features - mean_[label]).transpose() *
                     covariance_matrix_[label].inverse() *
                     (features - mean_[label]);
    return result;
  }

  string NonLinearDiscriminant::generateCppCode(
      const string& function_name) const {
    string result;
    result += "#include <Eigen/Dense>\n";
    result += "#include <cmath>\n";
    result += "#include <map>\n";

    result += "using Eigen::VectorXd;\n";
    result += "using Eigen::MatrixXd;\n";
    result += "using std::map;\n";

    result += "double " + function_name +
    "(const Object& object, const double label) {\n";
    result += " VectorXd features(object.feature_count());\n";

    result += " for (int element_index = 0;\n";
    result += "   element_index < features.size();\n";
    result += "   ++element_index) {\n";
    result += "  features(element_index) = object[element_index];\n";
    result += " }\n";

    result += "int n = object.feature_count();\n";
    result += "map<double, MatrixXd> cov;\n";
    result += "double lab;\n";

    int n = mean_.begin()->second.size();

    for (map<double, MatrixXd>::const_iterator it = covariance_matrix_.begin();
         it != covariance_matrix_.end();
         ++it) {
      result += "lab = " + lexical_cast<string>(it->first) + ";\n";
      result += "cov[lab] = MatrixXd(n, n);\n";
      for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
          result += "cov[lab](" +
                    lexical_cast<string>(i) +
                    ", " +
                    lexical_cast<string>(j) +
                    ") = " +
                    lexical_cast<string>(it->second(i, j)) +
                    ";\n";
        }
      }
    }

    result += "map<double, VectorXd> mean;\n";
    for (map<double, VectorXd>::const_iterator it = mean_.begin();
         it != mean_.end();
         ++it) {
      result += "lab = " + lexical_cast<string>(it->first) + ";\n";
      result += "mean[lab] = VectorXd(n);\n";
      for (int i = 0; i < n; ++i) {
        result += "mean[lab](" +
                   lexical_cast<string>(i) +
                   ") = " +
                   lexical_cast<string>(it->second(i)) +
                   ";\n";
      }
    }

    result += "double res = log(pow(M_PI, - n / 2.0) *\n";
    result += "                    pow(cov[label].determinant(), -0.5));\n";
    result += "res -= 0.5 * (features - mean[label]).transpose() *\n";
    result += "                 cov[label].inverse() *\n";
    result += "                 (features - mean[label]);\n";
    result += "return res;\n";
    result += "}\n\n";

    return result;
  }
};
