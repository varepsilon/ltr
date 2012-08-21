// Copyright 2012 Yandex
#include <cmath>

#include "ltr/density_estimators/scorers/non_linear_discriminant.h"
#include "ltr/utility/eigen_converters.h"

using ltr::NonLinearDiscriminant;
using ltr::utility::InitEigenMatrix;
using ltr::utility::InitEigenVector;

namespace ltr {
  double NonLinearDiscriminant::estimate(const Object& object,
                                         const double label) {
    VectorXd features(object.feature_count());
    InitEigenVector(&features);
    for (int feature_index = 0;
         feature_index < object.feature_count();
         ++feature_index) {
      features[feature_index] = object[feature_index];
    }

    int feature_count = object.feature_count();
    double result = log(pow(M_PI, - feature_count / 2.0) *
                        pow(covariance_matrix_[label].determinant(), -0.5));
    result -= 0.5 * (features - mean_[label]).transpose() *
                     covariance_matrix_[label].inverse() *
                     (features - mean_[label]);
    return result;
  }
};
