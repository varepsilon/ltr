// Copyright 2011 Yandex School Practice

#include <gtest/gtest.h>

#include <vector>
#include <limits>

#include "ltr/data/object.h"
#include "ltr/learners/best_feature_learner.h"
#include "ltr/scorers/fake_scorer.h"
#include "ltr/measures/abs_error.h"
#include "ltr/data/utility/io_utility.h"
#include "ltr/data/data_set.h"
#include "ltr/scorers/utility/scorer_utility.h"

// The fixture for testing (contains data for tests).
class LearnersTest : public ::testing::Test {
  protected:
  virtual void SetUp() {
    // Code here will be called immediately after the constructor (right
    // before each test).
    learn_data = ltr::io_utility::loadDataSet<ltr::Object>(
        "data/imat2009/imat2009_learning.txt", "YANDEX");
    test_data = ltr::io_utility::loadDataSet<ltr::Object>(
            "data/imat2009/imat2009_test.txt", "YANDEX");
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test (right
    // before the destructor).
  }
  // objects, used in tests
  protected:
  ltr::DataSet<ltr::Object> learn_data;
  ltr::DataSet<ltr::Object> test_data;
};

// tests.
TEST_F(LearnersTest, TestingBestFeatureLearner) {
  ltr::Measure<ltr::Object>::Ptr pMeasure(new ltr::AbsError<ltr::Object>());
  ltr::BestFeatureLearner<ltr::Object> learner(pMeasure);
  learner.learn(learn_data);

  ltr::FakeScorer fakeScorer(std::numeric_limits<double>::max());
  ltr::utility::MarkDataSet(test_data, fakeScorer);
  double measureBefore = pMeasure->average(test_data);

  ltr::utility::MarkDataSet(test_data, learner.make());
  double measureAfter = pMeasure->average(test_data);

  EXPECT_LE(measureAfter, measureBefore) << "It can't be worth.\n";
};
