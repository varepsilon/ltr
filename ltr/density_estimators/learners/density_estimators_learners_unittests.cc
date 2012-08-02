// Copyright 2012 Yandex

#include <Eigen/Dense>

#include <map>
#include <vector>

#include "gtest/gtest.h"

#include "ltr/density_estimators/learners/fisher_discriminant_learner.h"
#include "ltr/density_estimators/learners/quadratic_discriminant_learner.h"
#include "ltr/density_estimators/learners/normal_naive_bayes_learner.h"
#include "ltr/density_estimators/scorers/base_probability_density_estimator.h"
#include "ltr/density_estimators/scorers/fisher_discriminant.h"
#include "ltr/density_estimators/scorers/non_linear_discriminant.h"
#include "ltr/utility/numerical.h"
#include "ltr/utility/shared_ptr.h"
#include "ltr/data/data_set.h"

using std::map;

using Eigen::VectorXd;
using Eigen::MatrixXd;


using ltr::DataSet;
using ltr::QuadraticDiscriminantLearner;
using ltr::NormalNaiveBayesLearner;
using ltr::BaseProbabilityDensityEstimator;
using ltr::FisherDiscriminant;
using ltr::FisherDiscriminantLearner;
using ltr::NonLinearDiscriminant;
using ltr::utility::DoubleEqual;

TEST(DensityEstimatorsLearnersTests, TestFisherDiscriminantLearner) {
  FisherDiscriminantLearner fisherLearner;

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

  ltr::utility::shared_ptr<FisherDiscriminant> fisherScorer;
  EXPECT_NO_THROW(fisherLearner.learn(data, fisherScorer.get()));
}

TEST(DensityEstimatorsLearnersTests, TestQuadraticDiscriminantLearner) {
  QuadraticDiscriminantLearner quadraticLearner;

  Object object1;
  Object object2;

  object1 << 1 << 2 << 3;
  object2 << 3 << 1 << 6;

  object1.set_actual_label(1.0);
  object2.set_actual_label(2.0);

  DataSet<Object> data;
  data.add(object1);
  data.add(object2);

  ltr::utility::shared_ptr<NonLinearDiscriminant> quadraticScorer;

  EXPECT_NO_THROW(quadraticLearner.learn(data, quadraticScorer.get()));
}

TEST(DensityEstimatorsLearnersTests, TestNormalNaiveBayesLearner) {
  NormalNaiveBayesLearner naiveLearner;

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

  ltr::utility::shared_ptr<NonLinearDiscriminant> naiveScorer;
  EXPECT_NO_THROW(naiveLearner.learn(data, naiveScorer.get()));
}
