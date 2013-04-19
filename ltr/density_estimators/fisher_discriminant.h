// Copyright 2012 Yandex
#ifndef LTR_DENSITY_ESTIMATORS_FISHER_DISCRIMINANT_H_
#define LTR_DENSITY_ESTIMATORS_FISHER_DISCRIMINANT_H_

#include <map>
#include <string>
#include <vector>

#include "contrib/include_Eigen.h"

#include "ltr/data/object.h"
#include "ltr/density_estimators/base_probability_density_estimator.h"

using std::vector;
using std::map;
using std::string;

using ltr::utility::lexical_cast;

using Eigen::VectorXd;
using Eigen::aligned_allocator;

using ltr::Object;
using ltr::BaseProbabilityDensityEstimator;

namespace ltr {

/**
 * Double value representing actual label of object.
 */
typedef double Label;

/**
 * Map representing the linear factor of the class with some certain label.
 */
typedef map<Label, VectorXd, std::less<Label>,
            aligned_allocator<std::pair<Label, VectorXd> > > LabelToLinearFactor;

/**
 * Map representing the linear bias of the class with some certain label.
 */
typedef map<Label, double> LabelToLinearBias;

/**
 * \brief Probability density estimator that uses
 * Fisher Linear Discriminant approach.
 */
class FisherDiscriminant : public BaseProbabilityDensityEstimator {
 public:
  FisherDiscriminant() {
  }

  explicit FisherDiscriminant(const ParametersContainer& parameters) {
  }

  FisherDiscriminant(const LabelToLinearFactor& linear_factors,
                     const LabelToLinearBias& linear_biases) :
    linear_factors_(linear_factors),
    linear_biases_(linear_biases) {
  }

  virtual double estimate(const Object& object, const double label) const;

  virtual string generateCppCode(const string& function_name) const;

 private:
  LabelToLinearFactor linear_factors_;
  LabelToLinearBias linear_biases_;
};
};

#endif  // LTR_DENSITY_ESTIMATORS_FISHER_DISCRIMINANT_H_
