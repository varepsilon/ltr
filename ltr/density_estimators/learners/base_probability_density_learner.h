// Copyright 2012 Yandex
#ifndef LTR_DENSITY_ESTIMATORS_LEARNERS_BASE_PROBABILITY_DENSITY_LEARNER_H_
#define LTR_DENSITY_ESTIMATORS_LEARNERS_BASE_PROBABILITY_DENSITY_LEARNER_H_

#include <map>

#include <Eigen/Dense>  // NOLINT

#include "ltr/utility/eigen_converters.h"
#include "ltr/utility/shared_ptr.h"
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
template<class TEstimator>
class BaseProbabilityDensityLearner {
 public:
  typedef ltr::utility::shared_ptr<BaseProbabilityDensityLearner> Ptr;
  virtual void learn(const DataSet<Object>& data_set) {
    learnImpl(data_set, &estimator_);
  }

  BaseProbabilityDensityLearner() {
  }

  virtual ~BaseProbabilityDensityLearner() {
  }

  typename TEstimator::Ptr makeSpecific() const {
    return typename TEstimator::Ptr(
      new TEstimator(estimator_));
  };

  BaseProbabilityDensityEstimator::Ptr make() const {
    return BaseProbabilityDensityEstimator::Ptr(makeSpecific());
  };

 private:

  /**
   * The function implements learning using given training data set.
   * \param data_set is the training data
   * \param estimator is the pointer to the generated density estimator
   */
  virtual void learnImpl(const DataSet<Object>& data_set,
                         TEstimator* estimator) = 0;

  TEstimator estimator_;
};
};

#endif  // LTR_DENSITY_ESTIMATORS_LEARNERS_BASE_PROBABILITY_DENSITY_LEARNER_H_
