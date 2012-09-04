// Copyright 2012 Yandex
#ifndef LTR_DENSITY_ESTIMATORS_LEARNERS_NORMAL_NAIVE_BAYES_LEARNER_H_
#define LTR_DENSITY_ESTIMATORS_LEARNERS_NORMAL_NAIVE_BAYES_LEARNER_H_

#include <Eigen/Dense>

#include <map>

#include "ltr/density_estimators/scorers/base_probability_density_estimator.h"
#include "ltr/density_estimators/learners/non_linear_discriminant_learner.h"
#include "ltr/data/data_set.h"
#include "ltr/data/object.h"

using ltr::BaseProbabilityDensityEstimator;
using ltr::NonLinearDiscriminantLearner;
using ltr::DataSet;
using ltr::Object;
using ltr::LabelToCovarianceMatrix;

namespace ltr {
/**
 * \class Probability density learner that uses Normal Naive Bayesian
 * approach to generate probability density estimator.
 */
class NormalNaiveBayesLearner : public NonLinearDiscriminantLearner {
 private:
  /**
   * Function calculates the variance of every class from the training data set
   */
  void calculateVariance(const DataSet<Object>& data_set,
                         map<double, VectorXd>* result);
  virtual void calculateCovarianceMatrix(const DataSet<Object>& data_set,
                                         LabelToCovarianceMatrix* result);
};
};

#endif  // LTR_DENSITY_ESTIMATORS_LEARNERS_NORMAL_NAIVE_BAYES_LEARNER_H_
