// Copyright 2012 Yandex
#include "ltr/density_estimators/learners/quadratic_discriminant_learner.h"
#include "ltr/utility/statistics_calculation.h"

using ltr::QuadraticDiscriminantLearner;
using ltr::LabelToMean;
using ltr::LabelToCovarianceMatrix;
using ltr::utility::CalculateLabelsCapacity;
using ltr::utility::LabelToCapacity;

namespace ltr {
void QuadraticDiscriminantLearner::calculateCovarianceMatrix(
    const DataSet<Object>& data_set,
    map<double, MatrixXd>* result) {
  LabelToMean mean;
  calculateMean(data_set, &mean);
  LabelToCapacity object_counter = CalculateLabelsCapacity(data_set);

  for (LabelToMean::iterator mean_iterator = mean.begin();
       mean_iterator != mean.end();
       ++mean_iterator) {
    (*result)[mean_iterator->first] = MatrixXd(mean_iterator->second.size(),
                                            mean_iterator->second.size());
  }

  for (int object_index = 0; object_index < data_set.size(); ++object_index) {
    double label = data_set[object_index].actual_label();
    ++object_counter[label];
    VectorXd features =
      StdVectorToEigenVector(data_set[object_index].features());
    (*result)[label] += (features - mean[label]) *
      (features - mean[label]).transpose();
  }

  for (map<double, int>::iterator counter_iterator = object_counter.begin();
       counter_iterator != object_counter.end();
       ++counter_iterator) {
    (*result)[counter_iterator->first] /= counter_iterator->second;
  }
}
};
