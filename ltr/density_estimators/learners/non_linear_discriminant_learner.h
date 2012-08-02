// Copyright 2012 Yandex
#ifndef LTR_DENSITY_ESTIMATORS_LEARNERS_NON_LINEAR_DISCRIMINANT_LEARNER_H_
#define LTR_DENSITY_ESTIMATORS_LEARNERS_NON_LINEAR_DISCRIMINANT_LEARNER_H_

#include <Eigen/Dense>

#include <map>

#include "ltr/density_estimators/scorers/base_probability_density_estimator.h"
#include "ltr/density_estimators/learners/base_probability_density_learner.h"
#include "ltr/data/data_set.h"
#include "ltr/data/object.h"

using Eigen::MatrixXd;
using Eigen::VectorXd;

using ltr::BaseProbabilityDensityEstimator;
using ltr::BaseProbabilityDensityLearner;
using ltr::DataSet;
using ltr::Object;

namespace ltr {
/**
 * \class Interface of discriminants that
 * can not be converted to linear separators
 */
class NonLinearDiscriminantLearner : public BaseProbabilityDensityLearner {
 protected:
  virtual void calculateMean(const DataSet<Object>& data_set,
                             LabelToMean* result);

 private:
  virtual void learnImpl(const DataSet<Object>& data_set,
                         BaseProbabilityDensityEstimator* estimator);
  virtual void calculateCovarianceMatrix(const DataSet<Object>& data_set,
                                         LabelToCovarianceMatrix* result) = 0;
};
};

#endif  // LTR_DENSITY_ESTIMATORS_LEARNERS_NON_LINEAR_DISCRIMINANT_LEARNER_H_
