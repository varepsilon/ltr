// Copyright 2011 Yandex School Practice

#include <gtest/gtest.h>

#include <vector>

#include "ltr/data/object.h"
#include "ltr/learners/learner.h"
#include "ltr/learners/best_feature_learner.h"
#include "ltr/measures/measure.h"
#include "ltr/parameters_container/parameters_container.h"

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
TEST_F(LearnersTest, TestingLearnerX) {
  ltr::Measure<ltr::Object>::Ptr pMeasure;
  ltr::BestFeatureLearner<ltr::Object> learner(pMeasure);
};
