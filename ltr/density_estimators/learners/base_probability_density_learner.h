// Copyright 2012 Yandex
#ifndef LTR_DENSITY_ESTIMATORS_LEARNERS_BASE_PROBABILITY_DENSITY_LEARNER_H_
#define LTR_DENSITY_ESTIMATORS_LEARNERS_BASE_PROBABILITY_DENSITY_LEARNER_H_

#include <Eigen/Dense>

#include <map>

#include "ltr/utility/eigen_converters.h"
#include "ltr/density_estimators/scorers/base_probability_density_estimator.h"
#include "ltr/data/data_set.h"
#include "ltr/data/object.h"

using std::map;

using Eigen::VectorXd;

using ltr::BaseProbabilityDensityEstimator;
using ltr::DataSet;
using ltr::Object;
using ltr::utility::StdVectorToEigenVector;

namespace ltr {

/**
 * \typedef double value representing actual label of object
 */
typedef double Label;

/**
 * \typedef map representing the mean of the class with
 * some certain label
 */
typedef map<Label, VectorXd> LabelToMean;

/**
 * \typedef map representing the covariance matrix
 *  of the class with some certain label
 */
typedef map<Label, MatrixXd> LabelToCovarianceMatrix;

/**
 * \class Simple interface of the probability
 * density estimation learner
 */
class BaseProbabilityDensityLearner {
 public:
  virtual void learn(const DataSet<Object>& data_set,
                     BaseProbabilityDensityEstimator* estimator) {
    learnImpl(data_set, estimator);
  }

  virtual ~BaseProbabilityDensityLearner() {
  }

 private:

  /**
   * The function implements learning using given training data set.
   * \param data_set is the training data
   * \param estimator is the pointer to the generated density estimator
   */
  virtual void learnImpl(const DataSet<Object>& data_set,
                         BaseProbabilityDensityEstimator* estimator) = 0;
};
};

#endif  // LTR_DENSITY_ESTIMATORS_LEARNERS_BASE_PROBABILITY_DENSITY_LEARNER_H_
