// Copyright 2012 Yandex

#include "ltr/metrics/manhattan_metric.h"

using ltr::ManhattanMetric;

namespace ltr {
double ManhattanMetric::distance(const ltr::Object& lhs,
                                 const ltr::Object& rhs) {
  if (lhs.feature_count() != rhs.feature_count()) {
    throw std::logic_error("Number of features must be equal");
  }

  double distance = 0;
  for (int feature_index = 0;
        feature_index < lhs.feature_count();
        ++feature_index) {
    distance += fabs(lhs[feature_index] - rhs[feature_index]);
  }
  return distance;
}

string ManhattanMetric::getDefaultAlias() const {
  return "Manhattan metric";
}
};
