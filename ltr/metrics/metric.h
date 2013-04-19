// Copyright 2012 Yandex

#ifndef LTR_METRICS_METRIC_H_
#define LTR_METRICS_METRIC_H_

#include <string>

#include "ltr/data/object.h"
#include "ltr/interfaces/aliaser.h"
#include "ltr/interfaces/parameterized.h"
#include "ltr/interfaces/serializable.h"
#include "ltr/utility/boost/shared_ptr.h"
#include "ltr/parameters_container/parameters_container.h"

using ltr::Object;
using ltr::Serializable;
using ltr::ParametersContainer;

namespace ltr {
/**
 * @section DESCRIPTION
 * This class allows to calculate most famous Metrics on pairs of objects.
 */
class BaseMetric : public Aliaser, public Parameterized, public Serializable {
 public:
  typedef ltr::utility::shared_ptr<BaseMetric> Ptr;
  typedef ltr::utility::shared_ptr<BaseMetric> BasePtr;

  BaseMetric() {
  }

  explicit BaseMetric(const ParametersContainer& parameters) {
  }

  /**
  * Value of metric(distance between objects)
  * @param lhs is a first element to calculate measure on
  * @param rhs is a second element to calculate measure on
  */
  virtual double distance(const ltr::Object& lhs,
                          const ltr::Object& rhs) const = 0 ;
};
};

#endif  // LTR_METRICS_METRIC_H_
