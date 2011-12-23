// Copyright 2011 Yandex School Practice

#include <gtest/gtest.h>

#include <vector>

#include "ltr/feature_converters/feature_converter.h"
#include "ltr/feature_converters/feature_subset_chooser.h"
#include "ltr/feature_converters/feature_normalizer_learner.h"
#include "ltr/data/utility/io_utility.h"
#include "ltr/learners/best_feature_learner.h"
#include "ltr/measures/abs_error.h"

// The fixture for testing (contains data for tests).
class FeatureConvertersTest : public ::testing::Test {
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
  static const size_t bestFeatureIndex = 16;
};

// tests.
TEST_F(FeatureConvertersTest, TestingFeatureSubsetChooser) {
  std::vector<size_t> indexes;

  for (size_t featureIdx = 0; featureIdx < learn_data.size(); ++featureIdx) {
    if (featureIdx == bestFeatureIndex) {
      continue;
    }
    indexes.push_back(featureIdx);
  }

  ltr::FeatureSubsetChooser::Ptr pSubsetChooser(
      new ltr::FeatureSubsetChooser(indexes));

  ltr::Measure<ltr::Object>::Ptr pMeasure(new ltr::AbsError<ltr::Object>());
  ltr::BestFeatureLearner<ltr::Object> learner(pMeasure);

  learner.addFeatureConverter(pSubsetChooser);

  learner.learn(learn_data);

  ltr::utility::MarkDataSet(learn_data, learner.make());
  double withoutBestFeatureMeasure = pMeasure->average(learn_data);

  ltr::OneFeatureScorer scorerByBestFeature(bestFeatureIndex);
  ltr::utility::MarkDataSet(learn_data, scorerByBestFeature);

  double bestFeatureMeasure = pMeasure->average(learn_data);

  EXPECT_GE(withoutBestFeatureMeasure, bestFeatureMeasure);
};

TEST_F(FeatureConvertersTest, TestingFeatureNormalisationStatistics) {
  ltr::FeatureNormalizerLearner<ltr::Object> normalizerLearner;

  ltr::DataSet<ltr::Object> l_data(ltr::FeatureInfo(3));
  ltr::Object o1;
  ltr::Object o2;
  ltr::Object o3;
  o1 << 1.0 << 2.0 << 3.0;
  o2 << 3.0 << 1.0 << 2.0;
  o3 << 2.0 << 3.0 << 1.0;
  l_data.add(o1);
  l_data.add(o2);
  l_data.add(o3);

  normalizerLearner.learn(l_data);

  for (size_t featureIdx = 0;
      featureIdx < l_data.featureCount();
      ++featureIdx) {
    ltr::PerFeatureLinearConverter * pConv =
        (ltr::PerFeatureLinearConverter *)(normalizerLearner.make().get());
    double coef = pConv->getCoefficient(featureIdx);
    double shift = pConv->getShift(featureIdx);
    EXPECT_DOUBLE_EQ(coef, 0.5);
    EXPECT_DOUBLE_EQ(shift, -0.5);
  }
}

TEST_F(FeatureConvertersTest, TestingFeatureNormalisationNoFailure) {
  ltr::FeatureNormalizerLearner<ltr::Object> normalizerLearner;
  EXPECT_NO_THROW(normalizerLearner.learn(learn_data));

  ltr::Measure<ltr::Object>::Ptr pMeasure(new ltr::AbsError<ltr::Object>());
  ltr::BestFeatureLearner<ltr::Object> learner(pMeasure);

  learner.addFeatureConverter(normalizerLearner.make());
  EXPECT_NO_THROW(learner.learn(learn_data));
  EXPECT_NO_THROW(ltr::utility::MarkDataSet(learn_data, learner.make()));
};
