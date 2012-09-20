// Copyright 2012 Yandex

#ifndef LTR_PREDICTIONS_AGGREGATORS_PREDICTIONS_AGGREGATOR_H_
#define LTR_PREDICTIONS_AGGREGATORS_PREDICTIONS_AGGREGATOR_H_

#include <vector>

#include "ltr/utility/shared_ptr.h"
#include "ltr/interfaces/serializable.h"
#include "ltr/interfaces/parameterized.h"

using std::vector;

using ltr::Serializable;
using ltr::Parameterized;

namespace ltr {
/**
  * Performs aggregation of objects weights and labels  
  */
class PredictionsAggregator : public Serializable, public Parameterized {
 public:
  typedef ltr::utility::shared_ptr<PredictionsAggregator> Ptr;
  virtual double aggregate(const vector<double>& labels,
                           const vector<double>& weights) = 0;
  virtual ~PredictionsAggregator() {}
};
};

#endif  // LTR_PREDICTIONS_AGGREGATORS_PREDICTIONS_AGGREGATOR_H_
