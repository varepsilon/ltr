// Copyright 2012 Yandex

#ifndef LTR_METRICS_MANHATTAN_METRIC_H_
#define LTR_METRICS_MANHATTAN_METRIC_H_

#include <stdexcept>
#include <string>

#include "ltr/metrics/metric.h"

using std::string;

namespace ltr {
/**
 * Manhattan metric on two objects
 */
class ManhattanMetric : public BaseMetric {
 public:
  typedef ltr::utility::shared_ptr<ManhattanMetric> Ptr;
  virtual double distance(const ltr::Object& lhs, const ltr::Object& rhs);

 private:
  virtual string getDefaultAlias() const;
};
};

#endif  // LTR_METRICS_MANHATTAN_METRIC_H_
