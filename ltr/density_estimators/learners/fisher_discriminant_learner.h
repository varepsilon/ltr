// Copyright 2012 Yandex
#ifndef LTR_DENSITY_ESTIMATORS_LEARNERS_FISHER_DISCRIMINANT_LEARNER_H_
#define LTR_DENSITY_ESTIMATORS_LEARNERS_FISHER_DISCRIMINANT_LEARNER_H_

#include <Eigen/Dense>

#include <map>

#include "ltr/density_estimators/scorers/base_probability_density_estimator.h"
#include "ltr/density_estimators/scorers/fisher_discriminant.h"
#include "ltr/density_estimators/learners/base_probability_density_learner.h"

#include "ltr/data/data_set.h"
#include "ltr/data/object.h"

using Eigen::MatrixXd;
using Eigen::VectorXd;

using ltr::BaseProbabilityDensityEstimator;
using ltr::BaseProbabilityDensityLearner;
using ltr::FisherDiscriminant;
using ltr::DataSet;
using ltr::Object;

using ltr::LabelToMean;
using ltr::LabelToLinearFactor;
using ltr::LabelToLinearBias;

namespace ltr {
/**
 * \class Probability density learner that uses Fisher discriminant
 * to generate probability density estimator. To learn more visit
 * http://www.ics.uci.edu/~welling/classnotes/papers_class/Fisher-LDA.pdf
 */
class FisherDiscriminantLearner : public
  BaseProbabilityDensityLearner<FisherDiscriminant> {
 public:
  FisherDiscriminantLearner() {
  }
 private:
  /**
   * Function that calculates the mean vector of every class of the
   * training data set
   */
  void calculateMean(const DataSet<Object>& data_set,
                     LabelToMean* result);

  /**
   * Function calculates the overall covariance matrix of the training
   * data set
   */
  void calculateCovarianceMatrix(const DataSet<Object>& data_set,
                                 const LabelToMean& mean,
                                 MatrixXd* result);

  /**
   * Function finds linear factors and biases that help to simplify
   * estimation process
   */
  void findLinearFactorsAndBiases(const DataSet<Object>& data_set,
                                  LabelToLinearFactor* linear_factors,
                                  LabelToLinearBias* linear_biases);

  /**
   * Implementation of the learning of density estimator
   */
  virtual void learnImpl(const DataSet<Object>& data_set,
                         FisherDiscriminant* estimator);
};
};

#endif  // LTR_DENSITY_ESTIMATORS_LEARNERS_FISHER_DISCRIMINANT_LEARNER_H_
