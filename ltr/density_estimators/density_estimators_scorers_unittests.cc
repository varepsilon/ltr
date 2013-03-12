// Copyright 2012 Yandex

#include <map>
#include <vector>

#include "gtest/gtest.h"

#include "contrib/include_Eigen.h"

#include "ltr/density_estimators/base_probability_density_estimator.h"
#include "ltr/density_estimators/fisher_discriminant.h"
#include "ltr/density_estimators/non_linear_discriminant.h"
#include "ltr/utility/numerical.h"

using std::map;

using Eigen::VectorXd;
using Eigen::MatrixXd;
using Eigen::aligned_allocator;

using ltr::BaseProbabilityDensityEstimator;
using ltr::FisherDiscriminant;
using ltr::NonLinearDiscriminant;
using ltr::utility::DoubleEqual;
using ltr::utility::doubleVectorXdMap;
using ltr::utility::doubleMatrixXdMap;

TEST(DensityEstimatorsScorersTests, TestFisherDiscriminant) {
  VectorXd factor1(3);
  factor1 << 1.0, 2.0, 3.0;

  VectorXd factor2(3);
  factor2 << 3.0, -1.0, 1.0;

  doubleVectorXdMap factors;
  factors[1.0] = factor1;
  factors[2.0] = factor2;

  map<double, double> biases;
  biases[1.0] = 1.0;
  biases[2.0] = 2.0;

  FisherDiscriminant fisher(factors, biases);

  Object object;
  object << 2 << 5 << 1;
  EXPECT_TRUE(DoubleEqual(fisher.estimate(object, 1.0),
                          2.0 + 10.0 + 3.0 + 1.0));
}

TEST(DensityEstimatorsScorersTests, TestNonLinearDiscriminant) {
  MatrixXd matrix1(3, 3);
  matrix1 << 1, 0, 0,
             0, 2, 0,
             0, 0, 3;
  doubleMatrixXdMap covariance;
  covariance[1.0] = matrix1;

  VectorXd vector1(3);
  vector1 << 5, 1, 2;
  doubleVectorXdMap mean;
  mean[1.0] = vector1;

  NonLinearDiscriminant discriminant(covariance, mean);

  Object object;
  object << 2 << 5 << 1;
  EXPECT_NO_THROW(discriminant.estimate(object, 1.0));
}
