// Copyright 2012 Yandex

#ifndef LTR_AGGREGATORS_AGGREGATOR_H_
#define LTR_AGGREGATORS_AGGREGATOR_H_

#include <vector>

#include "ltr/utility/shared_ptr.h"
#include "ltr/interfaces/serializable.h"

using std::vector;

using ltr::Serializable;

namespace ltr {
/**
  * Performs aggregation of objects weights and labels  
  */
class Aggregator : public Serializable {
 public:
  typedef ltr::utility::shared_ptr<Aggregator> Ptr;
  virtual double aggregate(const vector<double>& labels,
                           const vector<double>& weights) = 0;
  virtual ~Aggregator() {}
};
};

#endif  // LTR_AGGREGATORS_AGGREGATOR_H_
