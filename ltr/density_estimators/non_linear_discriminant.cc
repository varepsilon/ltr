// Copyright 2012 Yandex

#include "ltr/density_estimators/non_linear_discriminant.h"

#include <cmath>
#include <sstream>

#include "ltr/utility/eigen_converters.h"

using std::stringstream;

using ltr::NonLinearDiscriminant;
using ltr::utility::InitEigenMatrix;
using ltr::utility::InitEigenVector;
using ltr::utility::doubleVectorXdMap;
using ltr::utility::doubleMatrixXdMap;

namespace ltr {
  double NonLinearDiscriminant::estimate(const Object& object,
                                         const double label) const {
    VectorXd features(object.feature_count());
    InitEigenVector(&features);
    for (int feature_index = 0;
         feature_index < object.feature_count();
         ++feature_index) {
      features(feature_index) = object[feature_index];
    }

    int feature_count = object.feature_count();
    double result = log(pow(M_PI, - feature_count / 2.0) *
                        pow(covariance_matrix_.find(label)->second.determinant(), -0.5));
    result -= 0.5 * (features - mean_.find(label)->second).transpose() *
                     covariance_matrix_.find(label)->second.inverse() *
                     (features - mean_.find(label)->second);
    return result;
  }

  string NonLinearDiscriminant::generateCppCode(
      const string& function_name) const {
    stringstream code;
    code
      << "#include <cmath>\n"
      << "#include <map>\n"
      << "\n"
      << "#include \"contrib/include_Eigen.h\"\n"
      << "\n"
      << "using Eigen::VectorXd;\n"
      << "using Eigen::MatrixXd;\n"
      << "using Eigen::aligned_allocator;\n"
      << "using std::map;\n"
      << "\n"
      << "double " << function_name << "(const Object& object,\n"
      << "    double label) {\n"
      << "  VectorXd features(object.feature_count());\n"
      << "\n"
      << "  for (int element_index = 0;\n"
      << "    element_index < features.size();\n"
      << "    ++element_index) {\n"
      << "   features(element_index) = object[element_index];\n"
      << "  }\n"
      << "\n"
      << "  int n = object.feature_count();\n"
      << "  map<double, MatrixXd, std::less<double>,\n"
      << "    aligned_allocator<std::pair<double, MatrixXd> > > cov;\n"
      << "  double lab;\n"
      << "\n";

    int n = mean_.begin()->second.size();

    for (doubleMatrixXdMap::const_iterator it =
             covariance_matrix_.begin();
         it != covariance_matrix_.end();
         ++it) {
      code
        << "  lab = " + lexical_cast<string>(it->first) + ";\n"
        << "  cov[lab] = MatrixXd(n, n);\n";
      for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
          code
            << "  cov[lab](" << lexical_cast<string>(i) << ", "
            << lexical_cast<string>(j) << ") = "
            << lexical_cast<string>(it->second(i, j)) << ";\n";
        }
      }
    }

    code
      << "\n"
      << "  map<double, VectorXd, std::less<double>,\n"
      << "    aligned_allocator<std::pair<double, VectorXd> > > mean;\n";
    for (doubleVectorXdMap::const_iterator it = mean_.begin();
         it != mean_.end();
         ++it) {
      code
        << "  lab = " + lexical_cast<string>(it->first) + ";\n"
        << "  mean[lab] = VectorXd(n);\n";
      for (int i = 0; i < n; ++i) {
        code
          << "  mean[lab](" << lexical_cast<string>(i) << ") = "
          << lexical_cast<string>(it->second(i)) << ";\n";
      }
    }

    code
      << "\n"
      << "  double res = log(pow(M_PI, - n / 2.0) *\n"
      << "    pow(cov[label].determinant(), -0.5));\n"
      << "  res -= 0.5 * (features - mean[label]).transpose() *\n"
      << "    cov[label].inverse() * (features - mean[label]);\n"
      << "  return res;\n"
      << "}\n\n";

    return code.str();
  }
};
