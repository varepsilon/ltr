// Copyright 2012 Yandex
#include "ltr/density_estimators/learners/normal_naive_bayes_learner.h"
#include "ltr/utility/statistics_calculation.h"
#include "ltr/utility/eigen_converters.h"

using ltr::NormalNaiveBayesLearner;
using ltr::LabelToMean;
using ltr::LabelToCovarianceMatrix;
using ltr::utility::CalculateLabelsCapacity;
using ltr::utility::LabelToCapacity;
using ltr::utility::SplitDataSetByActualLabel;
using ltr::utility::CalculateFeaturesVariance;
using ltr::utility::StdVectorToEigenVector;

namespace ltr {
void NormalNaiveBayesLearner::calculateVariance(
    const DataSet<Object>& data_set,
    map<double, VectorXd>* result) {

  vector<DataSet<Object> > splitted_data;
  SplitDataSetByActualLabel(data_set, &splitted_data);
  for (int subset_index = 0;
       subset_index < splitted_data.size();
       ++subset_index) {
    vector<double> variance;
    CalculateFeaturesVariance(splitted_data[subset_index], &variance);
    double label = splitted_data[subset_index][0].actual_label();
    (*result)[label] = StdVectorToEigenVector(variance);
  }
}

void NormalNaiveBayesLearner::calculateCovarianceMatrix(
    const DataSet<Object>& data_set,
    LabelToCovarianceMatrix* result) {
  LabelToMean mean;
  map<double, VectorXd> variance;
  this->calculateMean(data_set, &mean);
  calculateVariance(data_set, &variance);

  for (map<double, VectorXd>::iterator variance_iterator = variance.begin();
       variance_iterator != variance.end();
       ++variance_iterator) {
    double label = variance_iterator->first;
    int features_count = variance_iterator->second.size();
    (*result)[label] = MatrixXd(features_count, features_count);
    for (int feature_index = 0;
         feature_index < features_count;
         ++feature_index) {
      (*result)[label](feature_index, feature_index) =
        variance[label](feature_index);
    }
  }
}
};
