// Copyright 2012 Yandex
#ifndef LTR_DENSITY_ESTIMATORS_NON_LINEAR_DISCRIMINANT_H_
#define LTR_DENSITY_ESTIMATORS_NON_LINEAR_DISCRIMINANT_H_

#include <Eigen/Dense>

#include <map>
#include <string>
#include <vector>


#include "ltr/data/object.h"
#include "ltr/density_estimators/base_probability_density_estimator.h"
#include "ltr/density_estimators/base_probability_density_learner.h"

using std::map;
using std::string;
using std::vector;

using ltr::utility::lexical_cast;

using Eigen::MatrixXd;
using Eigen::VectorXd;

using ltr::Object;
using ltr::BaseProbabilityDensityEstimator;
using ltr::LabelToMean;
using ltr::LabelToCovarianceMatrix;

namespace ltr {

/**
 * \brief probability density estimator that cannot be represented
 * as linear separator
 */
class NonLinearDiscriminant : public BaseProbabilityDensityEstimator {
 public:
  NonLinearDiscriminant() {
  }

  explicit NonLinearDiscriminant(const ParametersContainer& parameters) {
  }

  NonLinearDiscriminant(const LabelToCovarianceMatrix& covariance_matrix,
                        const LabelToMean& mean) :
    covariance_matrix_(covariance_matrix),
    mean_(mean) {
  }

  virtual double estimate(const Object& object, const double label) const;

  virtual string generateCppCode(const string& function_name) const;

 private:

  LabelToCovarianceMatrix covariance_matrix_;
  LabelToMean mean_;
};
};

#endif  // LTR_DENSITY_ESTIMATORS_NON_LINEAR_DISCRIMINANT_H_
