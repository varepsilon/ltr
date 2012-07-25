// Copyright 2012 Yandex

#ifndef LTR_METRICS_EUCLIDEAN_METRIC_H_
#define LTR_METRICS_EUCLIDEAN_METRIC_H_

#include <string>
#include <stdexcept>

#include "ltr/metrics/metric.h"

using std::string;

namespace ltr {
/**
 * Euclidean metric on two objects
 */
class EuclideanMetric : public BaseMetric {
 public:
  typedef ltr::utility::shared_ptr<EuclideanMetric> Ptr;
  virtual double distance(const ltr::Object& lhs, const ltr::Object& rhs);

 private:
  virtual string getDefaultAlias() const;
};
};

#endif  // LTR_METRICS_EUCLIDEAN_METRIC_H_
