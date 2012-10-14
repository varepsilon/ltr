// Copyright 2012 Yandex
#ifndef LTR_DENSITY_ESTIMATORS_BASE_PROBABILITY_DENSITY_ESTIMATOR_H_
#define LTR_DENSITY_ESTIMATORS_BASE_PROBABILITY_DENSITY_ESTIMATOR_H_

#include "ltr/data/object.h"
#include "ltr/scorers/scorer.h"
#include "ltr/utility/shared_ptr.h"
#include "ltr/interfaces/parameterized.h"
#include "ltr/interfaces/serializable.h"
#include "ltr/parameters_container/parameters_container.h"

using ltr::Object;
using ltr::Parameterized;
using ltr::ParametersContainer;
using ltr::Serializable;

namespace ltr {

/**
 * \brief Simple interface of the probability density estimator
 */
class BaseProbabilityDensityEstimator
  : public Parameterized, public Serializable {
 public:
  typedef ltr::utility::shared_ptr<BaseProbabilityDensityEstimator> Ptr;
  virtual double estimate(const Object& object, const double label) = 0;
  virtual ~BaseProbabilityDensityEstimator() {
  }
};
};

#endif  // LTR_DENSITY_ESTIMATORS_BASE_PROBABILITY_DENSITY_ESTIMATOR_H_
