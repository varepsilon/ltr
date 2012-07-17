// Copyright 2012 Yandex

#include <gtest/gtest.h>

#include <boost/filesystem.hpp>

#include <vector>
#include <limits>
#include <string>

#include "ltr/data/object.h"
#include "ltr/learners/best_feature_learner/best_feature_learner.h"
#include "ltr/scorers/fake_scorer.h"
#include "ltr/measures/abs_error.h"
#include "ltr/measures/ndcg.h"
#include "ltr/measures/dcg.h"
#include "ltr/data/utility/io_utility.h"
#include "ltr/data/data_set.h"
#include "ltr/feature_converters/nan_to_zero_converter.h"
#include "ltr/learners/linear_learner/linear_learner.h"
#include "ltr/scorers/linear_scorer.h"

using boost::filesystem::path;

using std::string;

using ltr::io_utility::loadDataSet;

using ltr::FeatureConverter;
using ltr::NanToZeroConverterLearner;
using ltr::Object;
using ltr::DataSet;

class LearnersTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    string learn_data_file_name =
        path("data/imat2009/imat2009_learning_small.txt").string();
    learn_data = loadDataSet<Object>(learn_data_file_name, "YANDEX");

    string test_data_file_name =
        path("data/imat2009/imat2009_test_small.txt").string();
    test_data = loadDataSet<Object>(test_data_file_name, "YANDEX");
  }

  DataSet<Object> learn_data;
  DataSet<Object> test_data;
};

TEST_F(LearnersTest, TestingBestFeatureLearner) {
  NanToZeroConverterLearner<Object> nan_to_zero_converter;
  nan_to_zero_converter.learn(learn_data);
  FeatureConverter::Ptr remove_nan = nan_to_zero_converter.make();
  remove_nan->apply(learn_data, &learn_data);
  remove_nan->apply(test_data, &test_data);

  ltr::Measure<ltr::Object>::Ptr abs_error_measure(new ltr::AbsError());
  ltr::BestFeatureLearner<ltr::Object> learner(abs_error_measure);
  learner.learn(learn_data);

  ltr::FakeScorer fake_scorer(std::numeric_limits<double>::max());
  fake_scorer.markDataSet(test_data);
  double measure_before = abs_error_measure->average(test_data);

  learner.make()->markDataSet(test_data);
  double measure_after = abs_error_measure->average(test_data);

  EXPECT_LE(measure_after, measure_before) << "It can't be worth.\n";
};

TEST_F(LearnersTest, TestingLinearLearner) {
  DataSet<Object> data;
  Object object1, object2, object3;
  object1 << 1.2 << 1.9;
  object2 << 1.7 << 512.1;
  object3 << 5.9 << 0.2;

  data.add(object1, 1.2);
  data.add(object2, 0.5);
  data.add(object3, 7.2);

  ltr::LinearLearner<Object> learner;

  learner.learn(data);

  ltr::LinearScorer::Ptr scorer = learner.makeSpecific();

  std::cout << scorer->generateCppCode();
};
