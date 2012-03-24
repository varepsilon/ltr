// Copyright 2011 Yandex School Practice

#include <gtest/gtest.h>

#include <boost/filesystem/path.hpp>

#include <vector>

#include "ltr/feature_converters/feature_converter.h"
#include "ltr/feature_converters/feature_subset_chooser.h"
#include "ltr/feature_converters/feature_normalizer_learner.h"
#include "ltr/feature_converters/remove_nan_converter.h"
#include "ltr/data/utility/io_utility.h"
#include "ltr/learners/best_feature_learner.h"
#include "ltr/measures/abs_error.h"
#include "ltr/measures/average_precision.h"
#include "ltr/measures/dcg.h"
#include "ltr/parameters_container/parameters_container.h"

using ltr::RemoveNaNConverter;

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
  static const int bestFeatureIndex = 16;

  boost::filesystem::path path_to_learndata;
  boost::filesystem::path path_to_testdata;
};

// tests.
TEST_F(FeatureConvertersTest, TestingFeatureSubsetChooser) {
  std::vector<int> indexes;

    for (int featureIdx = 0;
      featureIdx < learn_data_pointwise.featureCount();
      ++featureIdx) {
    if (featureIdx == bestFeatureIndex) {
      continue;
    }
    indexes.push_back(featureIdx);
  }

  ltr::FeatureSubsetChooser::Ptr pSubsetChooser(
      new ltr::FeatureSubsetChooser(indexes));

  RemoveNaNConverter::Ptr pRemoveNaN(new RemoveNaNConverter());

  ltr::Measure<ltr::Object>::Ptr pMeasure(new ltr::AbsError());
  ltr::BestFeatureLearner<ltr::Object> learner(pMeasure);

  learner.addFeatureConverter(pRemoveNaN);
  learner.addFeatureConverter(pSubsetChooser);

  EXPECT_NO_THROW(learner.learn(learn_data_pointwise));

  ltr::utility::MarkDataSet(learn_data_pointwise, learner.make());
  double withoutBestFeatureMeasure = pMeasure->average(learn_data_pointwise);

  ltr::OneFeatureScorer scorerByBestFeature(bestFeatureIndex);
  scorerByBestFeature.addFeatureConverter(pRemoveNaN);
  ltr::utility::MarkDataSet(learn_data_pointwise, scorerByBestFeature);

  double bestFeatureMeasure = pMeasure->average(learn_data_pointwise);

  EXPECT_GE(withoutBestFeatureMeasure, bestFeatureMeasure);
};

TEST_F(FeatureConvertersTest, TestingFeatureNormalisationNoFailureObject) {
  ltr::FeatureNormalizerLearner<ltr::Object> normalizerLearner;
  EXPECT_NO_THROW(normalizerLearner.learn(learn_data_pointwise));

  ltr::Measure<ltr::Object>::Ptr pMeasure(new ltr::AbsError());
  ltr::BestFeatureLearner<ltr::Object> learner(pMeasure);

  learner.addFeatureConverter(normalizerLearner.makePtr());
  EXPECT_NO_THROW(learner.learn(learn_data_pointwise));
  EXPECT_NO_THROW(
      ltr::utility::MarkDataSet(learn_data_pointwise, learner.make()));
};


TEST_F(FeatureConvertersTest, TestingFeatureNormalisationNoFailureObjectList) {
  ltr::FeatureNormalizerLearner<ltr::ObjectList> normalizerLearner;
  EXPECT_NO_THROW(normalizerLearner.learn(learn_data_listwise));

  ltr::Measure<ltr::ObjectList>::Ptr pMeasure(new ltr::DCG());
  ltr::BestFeatureLearner<ltr::ObjectList> learner(pMeasure);

  learner.addFeatureConverter(normalizerLearner.makePtr());
  EXPECT_NO_THROW(learner.learn(learn_data_listwise));
  EXPECT_NO_THROW(
      ltr::utility::MarkDataSet(learn_data_listwise, learner.make()));
};
