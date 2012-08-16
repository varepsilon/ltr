// Copyright 2012 Yandex
#include "ltr/density_estimators/scorers/fisher_discriminant.h"

#include <stdexcept>

#include "ltr/density_estimators/learners/fisher_discriminant_learner.h"
#include "ltr/utility/statistics_calculation.h"
#include "ltr/utility/eigen_converters.h"

using std::logic_error;

using ltr::FisherDiscriminant;
using ltr::FisherDiscriminantLearner;
using ltr::LabelToMean;
using ltr::utility::CalculateLabelsCapacity;
using ltr::utility::LabelToCapacity;
using ltr::utility::SplitDataSetByActualLabel;
using ltr::utility::CalculateFeaturesMean;
using ltr::utility::StdVectorToEigenVector;

namespace ltr {
void FisherDiscriminantLearner::calculateMean(
    const DataSet<Object>& data_set,
    LabelToMean* result) {

  vector<DataSet<Object> > splitted_data;
  SplitDataSetByActualLabel(data_set, &splitted_data);

  for (int subset_index = 0;
       subset_index < splitted_data.size();
       ++subset_index) {
    double label = splitted_data[subset_index][0].actual_label();
    vector<double> mean;
    CalculateFeaturesMean(splitted_data[subset_index], &mean);
    (*result)[label] = StdVectorToEigenVector(mean);
  }
}

void FisherDiscriminantLearner::calculateCovarianceMatrix(
    const DataSet<Object>& data_set,
    const LabelToMean& mean,
    MatrixXd* result) {
  int features_count = mean.begin()->second.size();
  (*result) = MatrixXd(features_count, features_count);
  for (int object_index = 0; object_index < data_set.size(); ++object_index) {
    VectorXd features =
      StdVectorToEigenVector(data_set[object_index].features());
    LabelToMean::const_iterator mean_iterator =
      mean.find(data_set[object_index].actual_label());

    (*result) += (features - mean_iterator->second) *
                 (features - mean_iterator->second).transpose();
  }
  if (data_set.size() == mean.size()) {
    throw std::logic_error(
      "Number of classes must be less then number of objects");
  }
  (*result) /= static_cast<double>(data_set.size() - mean.size());
}

void FisherDiscriminantLearner::findLinearFactorsAndBiases(
    const DataSet<Object>& data_set,
    LabelToLinearFactor* linear_factors,
    LabelToLinearBias* linear_biases) {

  LabelToMean mean;
  MatrixXd covarianceMatrix;
  calculateMean(data_set, &mean);
  calculateCovarianceMatrix(data_set, mean, &covarianceMatrix);
  for (LabelToMean::iterator mean_iterator = mean.begin();
       mean_iterator != mean.end();
       ++mean_iterator) {
    (*linear_factors)[mean_iterator->first] =
      covarianceMatrix.transpose() * mean_iterator->second;
    (*linear_biases)[mean_iterator->first] =
      - 0.5 * mean_iterator->second.transpose() *
        (*linear_factors)[mean_iterator->first];
  }
}

void FisherDiscriminantLearner::learnImpl(
    const DataSet<Object>& data_set,
    FisherDiscriminant* estimator) {
  LabelToLinearFactor linear_factors;
  LabelToLinearBias linear_biases;
  findLinearFactorsAndBiases(data_set, &linear_factors, &linear_biases);
  *estimator = FisherDiscriminant(linear_factors, linear_biases);
}
};
