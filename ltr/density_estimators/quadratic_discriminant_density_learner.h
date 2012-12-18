// Copyright 2012 Yandex
#ifndef LTR_DENSITY_ESTIMATORS_QUADRATIC_DISCRIMINANT_DENSITY_LEARNER_H_
#define LTR_DENSITY_ESTIMATORS_QUADRATIC_DISCRIMINANT_DENSITY_LEARNER_H_

#include <Eigen/Dense>

#include <map>

#include "ltr/density_estimators/base_probability_density_estimator.h"
#include "ltr/density_estimators/non_linear_discriminant_density_learner.h"
#include "ltr/data/data_set.h"
#include "ltr/data/object.h"
#include "ltr/data/utility/data_set_converters.h"
#include "ltr/utility/statistics_calculation.h"

using ltr::BaseProbabilityDensityEstimator;
using ltr::NonLinearDiscriminantDensityLearner;
using ltr::DataSet;
using ltr::Object;
using ltr::LabelToCovarianceMatrix;
using ltr::LabelToMean;
using ltr::utility::LabelToCapacity;
using ltr::utility::extractAllObjects;

namespace ltr {
/**
 * \brief Probability density learner that uses Quadratic Discriminant
 * approach to generate probability density estimator. To learn more visit
 * http://en.wikipedia.org/wiki/Quadratic_classifier
 */
template<class TElement>
class QuadraticDiscriminantDensityLearner
  : public NonLinearDiscriminantDensityLearner<TElement> {
 public:
  QuadraticDiscriminantDensityLearner() {
  }

  explicit QuadraticDiscriminantDensityLearner(
    const ParametersContainer& parameters) {
  }

 private:
  virtual void calculateCovarianceMatrix(const DataSet<TElement>& data_set,
                                         LabelToCovarianceMatrix* result) {
    LabelToMean mean;
    this->calculateMean(data_set, &mean);
    LabelToCapacity object_counter = CalculateLabelsCapacity(data_set);

    for (LabelToMean::iterator mean_iterator = mean.begin();
         mean_iterator != mean.end();
         ++mean_iterator) {
      (*result)[mean_iterator->first] = MatrixXd(mean_iterator->second.size(),
                                                 mean_iterator->second.size());
      InitEigenMatrix(&(*result)[mean_iterator->first]);
    }

    DataSet<Object> data = extractAllObjects(data_set);
    for (int object_index = 0; object_index < data_set.size(); ++object_index) {
      double label = data[object_index].actual_label();
      ++object_counter[label];
      VectorXd features =
        StdVectorToEigenVector(data[object_index].features());
      (*result)[label] += (features - mean[label]) *
        (features - mean[label]).transpose();
    }

    for (map<double, int>::iterator counter_iterator = object_counter.begin();
         counter_iterator != object_counter.end();
         ++counter_iterator) {
      (*result)[counter_iterator->first] /= counter_iterator->second;
    }
  };
};
};

#endif  // LTR_DENSITY_ESTIMATORS_QUADRATIC_DISCRIMINANT_DENSITY_LEARNER_H_
