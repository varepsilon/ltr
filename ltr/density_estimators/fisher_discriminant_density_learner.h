// Copyright 2012 Yandex
#ifndef LTR_DENSITY_ESTIMATORS_FISHER_DISCRIMINANT_DENSITY_LEARNER_H_
#define LTR_DENSITY_ESTIMATORS_FISHER_DISCRIMINANT_DENSITY_LEARNER_H_

#include <map>
#include <stdexcept>
#include <vector>

#include "contrib/include_Eigen.h"

#include "ltr/density_estimators/base_probability_density_estimator.h"
#include "ltr/density_estimators/fisher_discriminant.h"
#include "ltr/density_estimators/base_probability_density_learner.h"

#include "ltr/data/data_set.h"
#include "ltr/data/object.h"
#include "ltr/data/utility/data_set_converters.h"

#include "ltr/utility/statistics_calculation.h"

using std::logic_error;

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

using ltr::utility::CalculateLabelsCapacity;
using ltr::utility::LabelToCapacity;
using ltr::utility::SplitDataSetByActualLabel;
using ltr::utility::CalculateFeaturesMean;
using ltr::utility::StdVectorToEigenVector;
using ltr::utility::InitEigenMatrix;
using ltr::utility::InitEigenVector;
using ltr::utility::extractAllObjects;

namespace ltr {
/**
 * \brief Probability density learner that uses Fisher discriminant
 * to generate probability density estimator.
 * 
 * To learn more visit http://www.ics.uci.edu/~welling/classnotes/papers_class/Fisher-LDA.pdf
 */
template<class TElement>
class FisherDiscriminantDensityLearner : public
  BaseProbabilityDensityLearner<TElement, FisherDiscriminant> {
 public:
  FisherDiscriminantDensityLearner() {
  }

  explicit FisherDiscriminantDensityLearner(
    const ParametersContainer& parameters) {
  }

 private:
  /**
   * Function that calculates the mean vector of every class of the
   * training data set
   */
  void calculateMean(const DataSet<TElement>& data_set,
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

  /**
   * Function calculates the overall covariance matrix of the training
   * data set
   */
  void calculateCovarianceMatrix(const DataSet<TElement>& data_set,
                                 const LabelToMean& mean,
                                 MatrixXd* result) {
    int features_count = mean.begin()->second.size();
    (*result) = MatrixXd(features_count, features_count);
    InitEigenMatrix(result);

    DataSet<Object> data = extractAllObjects(data_set);
    for (int object_index = 0; object_index < data.size(); ++object_index) {
      VectorXd features =
        StdVectorToEigenVector(data[object_index].features());
      LabelToMean::const_iterator mean_iterator =
        mean.find(data[object_index].actual_label());

      (*result) += (features - mean_iterator->second) *
                   (features - mean_iterator->second).transpose();
    }
    if (data.size() == mean.size()) {
      throw std::logic_error(
        "Number of classes must be less then number of objects");
    }
    (*result) /= static_cast<double>(data.size() - mean.size());
  }

  /**
   * Function finds linear factors and biases that help to simplify
   * estimation process
   */
  void findLinearFactorsAndBiases(const DataSet<TElement>& data_set,
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
  };

  /**
   * Implementation of the learning of density estimator
   */
  virtual void learnImpl(const DataSet<TElement>& data_set,
                         FisherDiscriminant* estimator) {
    LabelToLinearFactor linear_factors;
    LabelToLinearBias linear_biases;
    findLinearFactorsAndBiases(data_set, &linear_factors, &linear_biases);
    *estimator = FisherDiscriminant(linear_factors, linear_biases);
  };
};
};

#endif  // LTR_DENSITY_ESTIMATORS_FISHER_DISCRIMINANT_DENSITY_LEARNER_H_
