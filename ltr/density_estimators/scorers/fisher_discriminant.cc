// Copyright 2012 Yandex
#include <cmath>

#include "ltr/density_estimators/scorers/fisher_discriminant.h"
#include "ltr/utility/eigen_converters.h"

using ltr::FisherDiscriminant;
using ltr::utility::InitEigenVector;

namespace ltr {
  double FisherDiscriminant::estimate(const Object& object,
                                      const double label) {
    VectorXd features(object.feature_count());
    InitEigenVector(&features);

    for (int feature_index = 0;
         feature_index < object.feature_count();
         ++feature_index) {
      features(feature_index) = object[feature_index];
    }
    return features.dot(linear_factors_[label]) + linear_biases_[label];
  }
};
