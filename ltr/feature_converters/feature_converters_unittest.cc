// Copyright 2011 Yandex School Practice

#include <gtest/gtest.h>

#include <boost/filesystem/path.hpp>

#include <vector>

#include "ltr/feature_converters/feature_converter.h"
#include "ltr/feature_converters/feature_subset_chooser.h"
#include "ltr/feature_converters/feature_normalizer_learner.h"
#include "ltr/data/utility/io_utility.h"
#include "ltr/learners/best_feature_learner.h"
#include "ltr/measures/abs_error.h"
#include "ltr/parameters_container/parameters_container.h"

// The fixture for testing (contains data for tests).
class FeatureConvertersTest : public ::testing::Test {
  public:
  FeatureConvertersTest()
    :path_to_learndata("data/imat2009/imat2009_learning.txt"),
     path_to_testdata("data/imat2009/imat2009_test.txt") {}

  protected:
  virtual void SetUp() {
    // Code here will be called immediately after the constructor (right
    // before each test).
    learn_data_pointwise = ltr::io_utility::loadDataSet<ltr::Object>(
        path_to_learndata.string(), "YANDEX");
    test_data_pointwise = ltr::io_utility::loadDataSet<ltr::Object>(
        path_to_testdata.string(), "YANDEX");

    learn_data_listwise = ltr::io_utility::loadDataSet<ltr::ObjectList>(
        path_to_learndata.string(), "YANDEX");
    test_data_listwise = ltr::io_utility::loadDataSet<ltr::ObjectList>(
        path_to_testdata.string(), "YANDEX");
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test (right
    // before the destructor).
  }
  // objects, used in tests
  protected:
  ltr::DataSet<ltr::Object> learn_data_pointwise;
  ltr::DataSet<ltr::Object> test_data_pointwise;
  ltr::DataSet<ltr::ObjectList> learn_data_listwise;
  ltr::DataSet<ltr::ObjectList> test_data_listwise;
  static const size_t bestFeatureIndex = 16;

  boost::filesystem::path path_to_learndata;
  boost::filesystem::path path_to_testdata;
};

// tests.
TEST_F(FeatureConvertersTest, TestingFeatureSubsetChooser) {
  std::vector<size_t> indexes;

  for (size_t featureIdx = 0;
      featureIdx < learn_data_pointwise.featureCount();
      ++featureIdx) {
    if (featureIdx == bestFeatureIndex) {
      continue;
    }
    indexes.push_back(featureIdx);
  }

  ltr::FeatureSubsetChooser::Ptr pSubsetChooser(
      new ltr::FeatureSubsetChooser(indexes));

  ltr::Measure<ltr::Object>::Ptr pMeasure(new ltr::AbsError());
  ltr::BestFeatureLearner<ltr::Object> learner(pMeasure);

  learner.addFeatureConverter(pSubsetChooser);

  EXPECT_NO_THROW(learner.learn(learn_data_pointwise));

  ltr::utility::MarkDataSet(learn_data_pointwise, learner.make());
  double withoutBestFeatureMeasure = pMeasure->average(learn_data_pointwise);

  ltr::OneFeatureScorer scorerByBestFeature(bestFeatureIndex);
  ltr::utility::MarkDataSet(learn_data_pointwise, scorerByBestFeature);

  double bestFeatureMeasure = pMeasure->average(learn_data_pointwise);

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

TEST_F(FeatureConvertersTest, TestingFeatureNormalisationNoFailureObject) {
  ltr::FeatureNormalizerLearner<ltr::Object> normalizerLearner;
  EXPECT_NO_THROW(normalizerLearner.learn(learn_data_pointwise));

  ltr::Measure<ltr::Object>::Ptr pMeasure(new ltr::AbsError());
  ltr::BestFeatureLearner<ltr::Object> learner(pMeasure);

  learner.addFeatureConverter(normalizerLearner.make());
  EXPECT_NO_THROW(learner.learn(learn_data_pointwise));
  EXPECT_NO_THROW(
      ltr::utility::MarkDataSet(learn_data_pointwise, learner.make()));
};

// ltr::AbsError<ltr::ObjectList>() need to be replaced with other listwise
// measure. None implemented yet
/*
TEST_F(FeatureConvertersTest, TestingFeatureNormalisationNoFailureObjectList) {
  ltr::FeatureNormalizerLearner<ltr::ObjectList> normalizerLearner;
  EXPECT_NO_THROW(normalizerLearner.learn(learn_data_listwise));

  ltr::Measure<ltr::ObjectList>::Ptr
  pMeasure(new ltr::AbsError<ltr::ObjectList>());
  ltr::BestFeatureLearner<ltr::ObjectList> learner(pMeasure);

  learner.addFeatureConverter(normalizerLearner.make());
  EXPECT_NO_THROW(learner.learn(learn_data_listwise));
  EXPECT_NO_THROW(
      ltr::utility::MarkDataSet(learn_data_listwise, learner.make()));
};
*/
