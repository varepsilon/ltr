// Copyright 2012 Yandex
#include <cmath>
#include <sstream>

#include "ltr/density_estimators/fisher_discriminant.h"
#include "ltr/utility/typedefs.h"

using std::stringstream;

using ltr::FisherDiscriminant;
using ltr::utility::doubleVectorXdMap;

namespace ltr {
double FisherDiscriminant::estimate(const Object& object,
                                    const double label) const {
  VectorXd features = VectorXd::Zero(object.feature_count());

  for (int feature_index = 0;
       feature_index < object.feature_count();
       ++feature_index) {
    features(feature_index) = object[feature_index];
  }

  return features.dot(linear_factors_.find(label)->second) +
    linear_biases_.find(label)->second;
}

string FisherDiscriminant::generateCppCode(
    const string& function_name) const {
  stringstream code;
  code
    << "#include \"contrib/include_Eigen.h\"\n"
    << "#include <map>\n"
    << "\n"
    << "using Eigen::VectorXd;\n"
    << "using Eigen::aligned_allocator;\n"
    << "using std::map;\n"
    << "\n"
    << "double " << function_name << "(const Object& object, double label) {\n"
    << "  VectorXd features(object.feature_count());\n"
    << "  for (int element_index = 0;\n"
    << "       element_index < features.size();\n"
    << "       ++element_index) {\n"
    << "    features(element_index) = object[element_index];\n"
    << "  }\n"
    << "\n"
    << "  map<double, VectorXd, std::less<double>,\n"
    << "    aligned_allocator<std::pair<double, VectorXd> > > factors;\n"
    << "  int sz;\n"
    << "  double lab;\n"
    << "\n";
  for (doubleVectorXdMap::const_iterator it =
           linear_factors_.begin();
       it != linear_factors_.end();
       ++it) {
    code
      << "  lab = " + lexical_cast<string>(it->first) + ";\n"
      << "  sz = " + lexical_cast<string>(it->second.size()) + ";\n"
      << "  factors[lab] = VectorXd(sz);\n";
    for (int index = 0; index < it->second.size(); ++index) {
      code << "  factors[lab](" << lexical_cast<string>(index) << ") = "
        << lexical_cast<string>(it->second(index)) << ";\n";
    }
    code << "\n";
  }

  code << "  map<double, double> biases;\n";
  for (map<double, double>::const_iterator it = linear_biases_.begin();
       it != linear_biases_.end();
       ++it) {
    code << "  biases[" << lexical_cast<string>(it->first) << "] = "
      << lexical_cast<string>(it->second) << ";\n";
  }

  code
    << "\n"
    << "  for (int feature_index = 0;\n"
    << "       feature_index < object.feature_count();\n"
    << "       ++feature_index) {\n"
    << "    features(feature_index) = object[feature_index];\n"
    << "  }\n"
    << "\n"
    << "  return features.dot(factors[label]) + biases[label];\n"
    << "}\n\n";
  return code.str();
}
};
