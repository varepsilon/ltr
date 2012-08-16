// Copyright 2012 Yandex
#ifndef LTR_DENSITY_ESTIMATORS_SCORERS_NON_LINEAR_DISCRIMINANT_H_
#define LTR_DENSITY_ESTIMATORS_SCORERS_NON_LINEAR_DISCRIMINANT_H_

#include <Eigen/Dense>

#include <vector>
#include <map>

#include "ltr/data/object.h"
#include "ltr/density_estimators/scorers/base_probability_density_estimator.h"
#include "ltr/density_estimators/learners/base_probability_density_learner.h"

using std::vector;
using std::map;

using Eigen::MatrixXd;
using Eigen::VectorXd;

using ltr::Object;
using ltr::BaseProbabilityDensityEstimator;
using ltr::LabelToMean;
using ltr::LabelToCovarianceMatrix;

namespace ltr {

/**
 * \class probability density estimator that cannot be represented
 * as linear separator
 */
class NonLinearDiscriminant : public BaseProbabilityDensityEstimator {
 public:
  NonLinearDiscriminant() {
  }

  NonLinearDiscriminant(const LabelToCovarianceMatrix& covariance_matrix,
                        const LabelToMean& mean) :
    covariance_matrix_(covariance_matrix),
    mean_(mean) {
  }

  virtual double estimate(const Object& object, const double label);

 private:
  LabelToCovarianceMatrix covariance_matrix_;
  LabelToMean mean_;
};
};

#endif  // LTR_DENSITY_ESTIMATORS_SCORERS_NON_LINEAR_DISCRIMINANT_H_
