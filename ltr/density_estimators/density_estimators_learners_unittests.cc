// Copyright 2012 Yandex

#include <Eigen/Dense>

#include <map>
#include <vector>

#include "gtest/gtest.h"

#include "ltr/density_estimators/fisher_discriminant_density_learner.h"
#include "ltr/density_estimators/quadratic_discriminant_density_learner.h"
#include "ltr/density_estimators/normal_naive_bayes_density_learner.h"
#include "ltr/density_estimators/base_probability_density_estimator.h"
#include "ltr/density_estimators/fisher_discriminant.h"
#include "ltr/density_estimators/non_linear_discriminant.h"
#include "ltr/utility/numerical.h"
#include "ltr/utility/boost/shared_ptr.h"
#include "ltr/data/data_set.h"

using std::map;

using Eigen::VectorXd;
using Eigen::MatrixXd;


using ltr::DataSet;
using ltr::QuadraticDiscriminantDensityLearner;
using ltr::NormalNaiveBayesDensityLearner;
using ltr::BaseProbabilityDensityEstimator;
using ltr::FisherDiscriminant;
using ltr::FisherDiscriminantDensityLearner;
using ltr::NonLinearDiscriminant;
using ltr::utility::DoubleEqual;

TEST(DensityEstimatorsLearnersTests, TestFisherDiscriminantDensityLearner) {
  FisherDiscriminantDensityLearner<Object> fisherLearner;

  Object object1;
  Object object2;
  Object object3;

  object1 << 1 << 2 << 3;
  object2 << 3 << 1 << 6;
  object3 << -1 << 0 << 2;

  object1.set_actual_label(1.0);
  object2.set_actual_label(2.0);
  object3.set_actual_label(1.0);

  DataSet<Object> data;
  data.add(object1);
  data.add(object2);
  data.add(object3);

  EXPECT_NO_THROW(fisherLearner.learn(data));
}

TEST(DensityEstimatorsLearnersTests, TestQuadraticDiscriminantDensityLearner) {
  QuadraticDiscriminantDensityLearner<Object> quadraticLearner;

  Object object1;
  Object object2;

  object1 << 1 << 2 << 3;
  object2 << 3 << 1 << 6;

  object1.set_actual_label(1.0);
  object2.set_actual_label(2.0);

  DataSet<Object> data;
  data.add(object1);
  data.add(object2);

  EXPECT_NO_THROW(quadraticLearner.learn(data));
}

TEST(DensityEstimatorsLearnersTests, TestNormalNaiveBayesDensityLearner) {
  NormalNaiveBayesDensityLearner<Object> naiveLearner;

  Object object1;
  Object object2;
  Object object3;

  object1 << 1 << 1 << 2;
  object2 << 2 << 0 << 1;
  object3 << 3 << 1 << 3;

  object1.set_actual_label(1.0);
  object2.set_actual_label(3.0);
  object3.set_actual_label(3.0);

  DataSet<Object> data;
  data.add(object1);
  data.add(object2);
  data.add(object3);

  EXPECT_NO_THROW(naiveLearner.learn(data));
}
