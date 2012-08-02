// Copyright 2012 Yandex
#include <cmath>

#include "ltr/density_estimators/scorers/fisher_discriminant.h"

using ltr::FisherDiscriminant;

namespace ltr {
  double FisherDiscriminant::estimate(const Object& object,
                                      const double label) {
    VectorXd features(object.feature_count());
    for (int feature_index = 0;
         feature_index < object.feature_count();
         ++feature_index) {
      features(feature_index) = object[feature_index];
    }
    return log(features.dot(linear_factors_[label]) + linear_biases_[label]);
  }
};
