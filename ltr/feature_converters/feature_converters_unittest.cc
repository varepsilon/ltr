// Copyright 2011 Yandex School Practice

#include <gtest/gtest.h>

#include <boost/filesystem/path.hpp>

#include <vector>

#include "ltr/feature_converters/feature_converter.h"
#include "ltr/feature_converters/feature_sampler.h"
#include "ltr/feature_converters/feature_sampler_learner.h"
#include "ltr/feature_converters/feature_random_sampler_learner.h"
#include "ltr/feature_converters/feature_normalizer_learner.h"
#include "ltr/feature_converters/nan_to_zero_converter.h"
#include "ltr/data/utility/io_utility.h"
#include "ltr/learners/best_feature_learner/best_feature_learner.h"
#include "ltr/measures/abs_error.h"
#include "ltr/measures/average_precision.h"
#include "ltr/measures/dcg.h"
#include "ltr/parameters_container/parameters_container.h"
#include "ltr/feature_converters/utility/utility.h"

using ltr::utility::ApplyFeatureConverter;
using ltr::FeatureConverter;
using ltr::NanToZeroConverterLearner;

// The fixture for testing (contains data for tests).
class FeatureConvertersTest : public ::testing::Test {
  public:
  FeatureConvertersTest()
    :path_to_learndata("data/imat2009/imat2009_learning_small.txt"),
     path_to_testdata("data/imat2009/imat2009_test_small.txt") {}

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

    NanToZeroConverterLearner<ltr::Object>::Ptr nan_to_zero_converter
      (new NanToZeroConverterLearner<ltr::Object>);
    nan_to_zero_converter->learn(learn_data_pointwise);
    FeatureConverter::Ptr remove_NaN = nan_to_zero_converter->make();
    ApplyFeatureConverter(remove_NaN,
      learn_data_pointwise, &learn_data_pointwise);
    ApplyFeatureConverter(remove_NaN,
      test_data_pointwise, &test_data_pointwise);

    NanToZeroConverterLearner<ltr::ObjectList>::Ptr nan_to_zero_converter2
      (new NanToZeroConverterLearner<ltr::ObjectList>);
    nan_to_zero_converter2->learn(learn_data_listwise);
    FeatureConverter::Ptr remove_NaN2 = nan_to_zero_converter2->make();
    ApplyFeatureConverter(remove_NaN2,
      learn_data_listwise, &learn_data_listwise);
    ApplyFeatureConverter(remove_NaN2,
      test_data_listwise, &test_data_listwise);
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

  ltr::FeatureSamplerLearner<ltr::Object>::Ptr pSubsetChooserL(
    new ltr::FeatureSamplerLearner<ltr::Object>);
  pSubsetChooserL->setExistingParameter("INDICES", indexes);

  // remove NaN here is a code-dublicate from gtest fixture SetUp function
  NanToZeroConverterLearner<ltr::Object>::Ptr nan_to_zero_converter
    (new NanToZeroConverterLearner<ltr::Object>);
  nan_to_zero_converter->learn(learn_data_pointwise);
  FeatureConverter::Ptr remove_NaN = nan_to_zero_converter->make();

  ltr::Measure<ltr::Object>::Ptr pMeasure(new ltr::AbsError());
  ltr::BestFeatureLearner<ltr::Object> learner(pMeasure);

  learner.addFeatureConverter(nan_to_zero_converter);
  learner.addFeatureConverter(pSubsetChooserL);

  EXPECT_NO_THROW(learner.learn(learn_data_pointwise));

  ltr::utility::MarkDataSet(learn_data_pointwise, learner.make());
  double withoutBestFeatureMeasure = pMeasure->average(learn_data_pointwise);

  ltr::OneFeatureScorer scorerByBestFeature(bestFeatureIndex);
  scorerByBestFeature.addFeatureConverter(remove_NaN);
  ltr::utility::MarkDataSet(learn_data_pointwise, scorerByBestFeature);

  double bestFeatureMeasure = pMeasure->average(learn_data_pointwise);

  EXPECT_LE(withoutBestFeatureMeasure, bestFeatureMeasure);
};

TEST_F(FeatureConvertersTest, TestingFeatureNormalisationNoFailureObject) {
  ltr::FeatureNormalizerLearner<ltr::Object>::Ptr normalizerLearner
    (new ltr::FeatureNormalizerLearner<ltr::Object>);
  EXPECT_NO_THROW(normalizerLearner->learn(learn_data_pointwise));

  ltr::Measure<ltr::Object>::Ptr pMeasure(new ltr::AbsError());
  ltr::BestFeatureLearner<ltr::Object> learner(pMeasure);

  learner.addFeatureConverter(normalizerLearner);
  EXPECT_NO_THROW(learner.learn(learn_data_pointwise));
  EXPECT_NO_THROW(
      ltr::utility::MarkDataSet(learn_data_pointwise, learner.make()));
};


TEST_F(FeatureConvertersTest, TestingFeatureNormalisationNoFailureObjectList) {
  ltr::FeatureNormalizerLearner<ltr::ObjectList>::Ptr normalizerLearner
    (new ltr::FeatureNormalizerLearner<ltr::ObjectList>);
  EXPECT_NO_THROW(normalizerLearner->learn(learn_data_listwise));

  ltr::Measure<ltr::ObjectList>::Ptr pMeasure(new ltr::DCG());
  ltr::BestFeatureLearner<ltr::ObjectList> learner(pMeasure);

  learner.addFeatureConverter(normalizerLearner);
  EXPECT_NO_THROW(learner.learn(learn_data_listwise));
  EXPECT_NO_THROW(
      ltr::utility::MarkDataSet(learn_data_listwise, learner.make()));
};
