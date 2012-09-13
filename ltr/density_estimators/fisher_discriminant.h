// Copyright 2012 Yandex
#ifndef LTR_DENSITY_ESTIMATORS_FISHER_DISCRIMINANT_H_
#define LTR_DENSITY_ESTIMATORS_FISHER_DISCRIMINANT_H_

#include <Eigen/Dense>

#include <map>
#include <vector>

#include "ltr/data/object.h"
#include "ltr/density_estimators/base_probability_density_estimator.h"

using std::vector;
using std::map;

using Eigen::VectorXd;

using ltr::Object;
using ltr::BaseProbabilityDensityEstimator;

namespace ltr {

/**
 * \typedef double value representing actual label of object
 */
typedef double Label;

/**
 * \typedef map representing the linear factor
 * of the class with some certain label
 */
typedef map<Label, VectorXd> LabelToLinearFactor;

/**
 * \typedef map representing the linear bias
 * of the class with some certain label
 */
typedef map<Label, double> LabelToLinearBias;

/**
 * \class probability density estimator that uses
 * Fisher Linear Discriminant approach
 */
class FisherDiscriminant : public BaseProbabilityDensityEstimator {
 public:
  FisherDiscriminant() {
  }

  explicit FisherDiscriminant(const ParametersContainer& parameters) {
  }

  FisherDiscriminant(const map<Label, VectorXd>& linear_factors,
                     const map<Label, double>& linear_biases) :
    linear_factors_(linear_factors),
    linear_biases_(linear_biases) {
  }

  virtual double estimate(const Object& object, const double label);

 private:
  LabelToLinearFactor linear_factors_;
  LabelToLinearBias linear_biases_;
};
};

#endif  // LTR_DENSITY_ESTIMATORS_FISHER_DISCRIMINANT_H_
