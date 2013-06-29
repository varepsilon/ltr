// Copyright 2012 Yandex

#ifndef LTR_METRICS_EUCLIDEAN_METRIC_H_
#define LTR_METRICS_EUCLIDEAN_METRIC_H_

#include <string>
#include <stdexcept>

#include "ltr/metrics/metric.h"

using std::string;

namespace ltr {
/**
 * \brief Euclidean metric on two objects.
 */
class EuclideanMetric : public BaseMetric {
  ALLOW_SHARED_PTR_ONLY_CREATION(EuclideanMetric)
 public:

  EuclideanMetric() {
  }

  explicit EuclideanMetric(const ParametersContainer& parameters) {
  }

  virtual double distance(const ltr::Object& lhs, const ltr::Object& rhs) const;
  /**
   * The function generates code for the metric as cpp code function.
   *
   * \param function_name is the name of this function.
   */
  string generateCppCode(const string& function_name) const;

 private:
  virtual string getDefaultAlias() const;
};
};

#endif  // LTR_METRICS_EUCLIDEAN_METRIC_H_
