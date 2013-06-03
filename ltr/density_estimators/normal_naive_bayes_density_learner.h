// Copyright 2012 Yandex
#ifndef LTR_DENSITY_ESTIMATORS_NORMAL_NAIVE_BAYES_DENSITY_LEARNER_H_
#define LTR_DENSITY_ESTIMATORS_NORMAL_NAIVE_BAYES_DENSITY_LEARNER_H_

#include <map>
#include <vector>

#include "contrib/include_Eigen.h"

#include "ltr/density_estimators/base_probability_density_estimator.h"
#include "ltr/density_estimators/non_linear_discriminant_density_learner.h"
#include "ltr/data/data_set.h"
#include "ltr/data/object.h"
#include "ltr/utility/statistics_calculation.h"
#include "ltr/utility/typedefs.h"

using ltr::BaseProbabilityDensityEstimator;
using ltr::NonLinearDiscriminantDensityLearner;
using ltr::DataSet;
using ltr::Object;
using ltr::LabelToCovarianceMatrix;
using ltr::utility::CalculateFeaturesVariance;
using ltr::utility::doubleVectorXdMap;

namespace ltr {
/**
 * \brief Probability density learner that uses Normal Naive Bayesian
 * approach to generate probability density estimator.
 */
template<class TElement>
class NormalNaiveBayesDensityLearner
  : public NonLinearDiscriminantDensityLearner<TElement> {
 public:
  NormalNaiveBayesDensityLearner() {
  }

  explicit NormalNaiveBayesDensityLearner(
    const ParametersContainer& parameters) {
  }

 private:
  /**
   * \brief Function calculates the variance of every class from the training
   * data set.
   */
  void calculateVariance(const DataSet<TElement>& data_set,
                         doubleVectorXdMap* result) {
    vector<DataSet<Object> > splitted_data;
    SplitDataSetByActualLabel(data_set, &splitted_data);
    for (int subset_index = 0;
         subset_index < splitted_data.size();
         ++subset_index) {
      VectorXd variance;
      CalculateFeaturesVariance(splitted_data[subset_index], &variance);
      double label = splitted_data[subset_index][0].actual_label();
      (*result)[label] = variance;
    }
  }

  virtual void calculateCovarianceMatrix(const DataSet<TElement>& data_set,
                                         LabelToCovarianceMatrix* result) {
    LabelToMean mean;
    doubleVectorXdMap variance;
    this->calculateMean(data_set, &mean);
    calculateVariance(data_set, &variance);

    for (doubleVectorXdMap::iterator variance_iterator = variance.begin();
         variance_iterator != variance.end();
         ++variance_iterator) {
      double label = variance_iterator->first;
      int features_count = variance_iterator->second.size();
      (*result)[label] = MatrixXd::Zero(features_count, features_count);

      for (int feature_index = 0;
           feature_index < features_count;
           ++feature_index) {
        (*result)[label](feature_index, feature_index) =
          variance[label](feature_index);
      }
    }
  };
};
};

#endif  // LTR_DENSITY_ESTIMATORS_NORMAL_NAIVE_BAYES_DENSITY_LEARNER_H_
