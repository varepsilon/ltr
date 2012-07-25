// Copyright 2012 Yandex

#ifndef LTR_METRICS_METRIC_H_
#define LTR_METRICS_METRIC_H_

#include <string>

#include "ltr/data/object.h"
#include "ltr/interfaces/aliaser.h"
#include "ltr/interfaces/parameterized.h"
#include "ltr/interfaces/serializable.h"
#include "ltr/utility/shared_ptr.h"

using ltr::Object;
using ltr::Serializable;

namespace ltr {
/**
 * @section DESCRIPTION
 * This class allows to calculate most famous Metrics on pairs of objects. 
 */
class BaseMetric : public Aliaser, public Parameterized, public Serializable {
 public:
  typedef ltr::utility::shared_ptr<BaseMetric> Ptr;
  typedef ltr::utility::shared_ptr<BaseMetric> BasePtr;

  /**
  * Value of metric(distance between objects)
  * @param lhs and rhs - elements to calculate measure on
  */
  virtual double distance(const ltr::Object& lhs,
                          const ltr::Object& rhs) = 0;
};
};

#endif  // LTR_METRICS_METRIC_H_
