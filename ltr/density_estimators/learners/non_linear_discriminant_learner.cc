// Copyright 2012 Yandex
#include "ltr/density_estimators/learners/non_linear_discriminant_learner.h"
#include "ltr/density_estimators/scorers/non_linear_discriminant.h"
#include "ltr/utility/statistics_calculation.h"
#include "ltr/utility/eigen_converters.h"

using ltr::NonLinearDiscriminantLearner;
using ltr::NonLinearDiscriminant;
using ltr::utility::CalculateLabelsCapacity;
using ltr::utility::CalculateActualLabelToFeaturesMean;
using ltr::utility::StdVectorToEigenVector;

using ltr::LabelToMean;
using ltr::LabelToCovarianceMatrix;

namespace ltr {
void NonLinearDiscriminantLearner::calculateMean(
    const DataSet<Object>& data_set,
    LabelToMean* result) {
  map<double, vector<double> > means;
  CalculateActualLabelToFeaturesMean(data_set, &means);

  for (map<double, vector<double> >::iterator mean_iterator = means.begin();
       mean_iterator != means.end();
       ++mean_iterator) {
    (*result)[mean_iterator->first] =
      StdVectorToEigenVector(mean_iterator->second);
  }
}

void NonLinearDiscriminantLearner::learnImpl(
    const DataSet<Object>& data_set,
    BaseProbabilityDensityEstimator* estimator) {
  LabelToMean mean;
  LabelToCovarianceMatrix covariance_matrix;
  calculateMean(data_set, &mean);
  calculateCovarianceMatrix(data_set, &covariance_matrix);
  estimator = new NonLinearDiscriminant(covariance_matrix, mean);
}
};
