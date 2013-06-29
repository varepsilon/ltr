// Copyright 2012 Yandex
#ifndef LTR_DENSITY_ESTIMATORS_NON_LINEAR_DISCRIMINANT_DENSITY_LEARNER_H_
#define LTR_DENSITY_ESTIMATORS_NON_LINEAR_DISCRIMINANT_DENSITY_LEARNER_H_

#include <map>
#include <vector>

#include "contrib/include_Eigen.h"

#include "ltr/density_estimators/base_probability_density_estimator.h"
#include "ltr/density_estimators/base_probability_density_learner.h"
#include "ltr/density_estimators/non_linear_discriminant.h"
#include "ltr/data/data_set.h"
#include "ltr/data/object.h"
#include "ltr/data/utility/data_set_converters.h"

#include "ltr/utility/statistics_calculation.h"

using Eigen::MatrixXd;
using Eigen::VectorXd;

using ltr::BaseProbabilityDensityEstimator;
using ltr::BaseProbabilityDensityLearner;
using ltr::NonLinearDiscriminant;
using ltr::DataSet;
using ltr::Object;
using ltr::utility::extractAllObjects;
using ltr::utility::CalculateActualLabelToFeaturesMean;

namespace ltr {
/**
 * \brief Interface of discriminants that
 * can not be converted to linear separators
 */
template<class TElement>
class NonLinearDiscriminantDensityLearner : public
  BaseProbabilityDensityLearner<TElement, NonLinearDiscriminant> {
 protected:
  virtual void calculateMean(const DataSet<TElement>& data_set,
                             LabelToMean* result) {
    map<double, VectorXd> means;
    CalculateActualLabelToFeaturesMean(data_set, &means);

    for (map<double, VectorXd>::iterator mean_iterator = means.begin();
         mean_iterator != means.end();
         ++mean_iterator) {
      (*result)[mean_iterator->first] =
        mean_iterator->second;
    }
  };

 private:
  virtual void calculateCovarianceMatrix(const DataSet<TElement>& data_set,
                                         LabelToCovarianceMatrix* result) = 0;

  virtual void learnImpl(const DataSet<TElement>& data_set,
                         NonLinearDiscriminant::Ptr* estimator) {
    LabelToMean mean;
    LabelToCovarianceMatrix covariance_matrix;
    calculateMean(data_set, &mean);
    calculateCovarianceMatrix(data_set, &covariance_matrix);
    (*estimator) = new NonLinearDiscriminant(covariance_matrix, mean);
  };
};
};

#endif  // LTR_DENSITY_ESTIMATORS_NON_LINEAR_DISCRIMINANT_DENSITY_LEARNER_H_
