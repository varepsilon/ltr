// Copyright 2012 Yandex

#include "ltr/metrics/euclidean_metric.h"

using ltr::EuclideanMetric;

namespace ltr {
double EuclideanMetric::distance(const ltr::Object& lhs,
                                 const ltr::Object& rhs) {
  if (lhs.feature_count() != rhs.feature_count()) {
    ERR("Number of features must be equal");
    throw std::logic_error("Number of features must be equal");
  }

  double distance = 0;
  for (int feature_index = 0;
       feature_index < lhs.feature_count();
       ++feature_index) {
    distance += pow(lhs[feature_index] - rhs[feature_index], 2.0);
  }
  return sqrt(distance);
}

string EuclideanMetric::generateCppCode(const string& function_name) const {
  string result;
  result += "double " + function_name +
    "(const Object& lhs, const Object& rhs) {\n";
  result += " double result = 0;\n";
  result += " for (int index = 0; index < lhs.feature_count(); ++index) {\n";
  result += "   result += (lhs[index] - rhs[index])";
  result += " * (lhs[index] - rhs[index]);\n";
  result += " }\n";
  result += " return sqrt(result);\n";
  result += "}\n";
  return result;
}

string EuclideanMetric::getDefaultAlias() const {
  return "Euclidean metric";
}
};
