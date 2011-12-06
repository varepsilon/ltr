// Copyright 2011 Yandex School Practice

#include <gtest/gtest.h>

#include <vector>

#include "ltr/feature_converters/feature_converter.h"
#include "ltr/feature_converters/feature_subset_chooser.h"
#include "ltr/feature_converters/feature_normalizer_learner.h"

// The fixture for testing (contains data for tests).
class FeatureConvertersTest : public ::testing::Test {
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
TEST_F(FeatureConvertersTest, TestingFeatureSubsetChooser) {
};

TEST_F(FeatureConvertersTest, TestingFeatureNormalisation) {
  ltr::FeatureNormalizerLearner<ltr::ObjectList> norm_learner;
};
