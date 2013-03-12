// Copyright 2012 Yandex

#ifndef LTR_FEATURE_CONVERTERS_PCA_LEARNER_H_
#define LTR_FEATURE_CONVERTERS_PCA_LEARNER_H_

#include <vector>
#include <algorithm>
#include <limits>
#include <string>
#include <sstream>

#include "contrib/include_Eigen.h"

#include "ltr/feature_converters/linear_converter.h"
#include "ltr/feature_converters/feature_converter_learner.h"

#include "ltr/data/utility/data_set_statistics.h"
#include "ltr/utility/numerical.h"
#include "ltr/utility/eigen_converters.h"
#include "ltr/utility/boost/shared_ptr.h"

using std::vector;
using std::string;
using std::logic_error;

using ltr::LinearConverter;
using ltr::utility::getFeaturesAverageValues;
using ltr::utility::StdVectorToEigenVector;
using ltr::utility::DataSetToEigenMatrix;
using ltr::utility::InitEigenVector;

using Eigen::MatrixXd;
using Eigen::VectorXd;
using Eigen::JacobiSVD;
using Eigen::ComputeThinU;

namespace ltr {
/**
 * \brief Learner for principal components analisys feature converter.
 */
template <class TElement>
class PCALearner
  : public BaseFeatureConverterLearner<TElement, LinearConverter> {
 public:
  typedef ltr::utility::shared_ptr<PCALearner> Ptr;

  explicit PCALearner() {}

  explicit PCALearner(const ParametersContainer& parameters);

  virtual string toString() const;

 private:
  virtual void learnImpl(const DataSet<TElement>& data_set,
                         LinearConverter* linear_converter);

  virtual string getDefaultAlias() const;

  /**
   * \brief Computes optimal number of principal components using Kaiser's Rule.
   * \param singular_values vector of singular values of train dataset.
   */
  int kaiserRule(const VectorXd& singualar_values);
};

template <typename TElement>
PCALearner<TElement>::PCALearner(
    const ParametersContainer& parameters) {
  this->setParameters(parameters);
}

template <typename TElement>
void PCALearner<TElement>::learnImpl(const DataSet<TElement>& data_set,
                                     LinearConverter* linear_converter) {
  Features average_features;
  getFeaturesAverageValues(data_set, &average_features);
  VectorXd eigen_average_features = StdVectorToEigenVector(average_features);

  MatrixXd source_matrix = DataSetToEigenMatrix(data_set).transpose();

  for (int object_index = 0;
       object_index < source_matrix.cols();
       ++object_index) {
    source_matrix.col(object_index) =
      source_matrix.col(object_index) - eigen_average_features;
  }

  JacobiSVD<MatrixXd> jacobi_svd = source_matrix.jacobiSvd(ComputeThinU);
  MatrixXd matrix_U = jacobi_svd.matrixU();
  VectorXd singular_values = jacobi_svd.singularValues();
  int component_count = kaiserRule(singular_values);

  matrix_U.conservativeResize(matrix_U.rows(), component_count);
  linear_converter->set_factor(matrix_U.transpose());

  VectorXd shift(component_count);
  InitEigenVector(&shift);
  linear_converter->set_shift(shift);
}

template <typename TElement>
string PCALearner<TElement>::toString() const {
  return "PCALearner";
}

template <typename TElement>
string PCALearner<TElement>::getDefaultAlias() const {
  return "PCALearner";
}

template <typename TElement>
int PCALearner<TElement>::kaiserRule(const VectorXd& singular_values) {
  double trace = 0;
  for (int value_index = 0;
       value_index < (int)singular_values.size();
       ++value_index) {
    trace += singular_values[value_index];
  }
  trace /= static_cast<int>(singular_values.size());

  int singular_values_count = 0;
  for (int value_index = 0;
       value_index < static_cast<int>(singular_values.size());
       ++value_index) {
    if (singular_values[value_index]>trace) {
      ++singular_values_count;
    }
  }
  return singular_values_count;
}
};
#endif  // LTR_FEATURE_CONVERTERS_PCA_LEARNER_H_
