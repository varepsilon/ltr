// Copyright 2012 Yandex

#include "ltr/utility/eigen_converters.h"

namespace ltr {
namespace utility {
VectorXd StdVectorToEigenVector(const Object& std_vector) {
  VectorXd result(std_vector.feature_count());
  InitEigenVector(&result);
  for (int element_index = 0;
       element_index < std_vector.feature_count();
       ++element_index) {
    result(element_index) = std_vector[element_index];
  }
  return result;
}

VectorXd StdVectorToEigenVector(const vector<double>& std_vector) {
  VectorXd result(std_vector.size());
  InitEigenVector(&result);
  for (int element_index = 0;
       element_index < std_vector.size();
       ++element_index) {
    result(element_index) = std_vector[element_index];
  }
  return result;
}

void EigenVectorToStdVector(const VectorXd& eigen_vector,
                            vector<double>* std_vector) {
  int feature_count = eigen_vector.size();
  std_vector->resize(feature_count);
  for (int feature_index = 0; feature_index < feature_count; ++feature_index) {
    (*std_vector)[feature_index] = eigen_vector[feature_index];
  }
}

void EigenVectorToStdVector(const Object& eigen_vector,
                            vector<double>* std_vector) {
  int feature_count = eigen_vector.feature_count();
  std_vector->resize(feature_count);
  for (int feature_index = 0; feature_index < feature_count; ++feature_index) {
    (*std_vector)[feature_index] = eigen_vector[feature_index];
  }
}

void InitEigenMatrix(MatrixXd* eigen_matrix) {
  for (int row = 0; row < eigen_matrix->rows(); ++row) {
    for (int col = 0; col < eigen_matrix->cols(); ++col) {
      (*eigen_matrix)(row, col) = 0.0;
    }
  }
}

void InitEigenVector(VectorXd* eigen_vector) {
  for (int element_index = 0;
       element_index < eigen_vector->size();
       ++element_index) {
    (*eigen_vector)(element_index) = 0.0;
  }
}
};
};
