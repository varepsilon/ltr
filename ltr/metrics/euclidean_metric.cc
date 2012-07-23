// Copyright 2012 Yandex

#include "ltr/metrics/euclidean_metric.h"

using ltr::EuclideanMetric;

namespace ltr {
double EuclideanMetric::distance(const ltr::Object& lhs,
                                 const ltr::Object& rhs) {
  if (lhs.feature_count() != rhs.feature_count()) {
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

string EuclideanMetric::getDefaultAlias() const {
  return "Euclidean metric";
}
};
