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

  EuclideanMetric() {
  }

  explicit EuclideanMetric(const ParametersContainer& parameters) {
  }

  typedef ltr::utility::shared_ptr<EuclideanMetric> Ptr;
  virtual double distance(const ltr::Object& lhs, const ltr::Object& rhs) const;

  /** the function generates code for the metric as cpp code function
   * \param class_name the name for the class that would be created.
   */
  string generateCppCode(const string& function_name) const;

 private:
  virtual string getDefaultAlias() const;
};
};

#endif  // LTR_METRICS_EUCLIDEAN_METRIC_H_
