// Copyright 2011 Yandex School Practice

#include <gtest/gtest.h>

#include <vector>

#include "ltr/data/object.h"
#include "ltr/learners/best_feature_learner.h"
#include "ltr/measures/abs_error.h"
#include "ltr/data/utility/io_utility.h"
#include "ltr/data/data_set.h"

// The fixture for testing (contains data for tests).
class LearnersTest : public ::testing::Test {
    protected:
    virtual void SetUp() {
        // Code here will be called immediately after the constructor (right
        // before each test).
    }

    virtual void TearDown() {
        // Code here will be called immediately after each test (right
        // before the destructor).
    }
    // objects, used in tests
    protected:
};

// tests.
TEST_F(LearnersTest, TestingBestFeatureLearner) {
  ltr::Measure<ltr::Object>::Ptr pMeasure(new ltr::AbsError<ltr::Object>());
  ltr::BestFeatureLearner<ltr::Object> learner(pMeasure);

  ltr::DataSet<ltr::Object> learn_data = ltr::io_utility::loadDataSet<ltr::Object>(
      "data/imat2009/imat2009_learning.txt", "YANDEX");

  /* ltr::DataSet<ltr::Object> test_data = ltr::io_utility::loadDataSet<ltr::Object>(
        "data/imat2009/imat2009_test.txt", "YANDEX");

  learner.learn(learn_data);*/
};
