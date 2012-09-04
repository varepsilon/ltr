// Copyright 2012 Yandex

#include "ltr/utility/eigen_converters.h"

namespace ltr {
namespace utility {
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
