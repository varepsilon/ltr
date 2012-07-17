// Copyright 2011 Yandex

#include <gtest/gtest.h>
#include <vector>

#include "ltr/crossvalidation/crossvalidation.h"
#include "ltr/learners/best_feature_learner/best_feature_learner.h"
#include "ltr/data/object.h"
#include "ltr/data/feature_info.h"
#include "ltr/measures/abs_error.h"
#include "ltr/measures/measure.h"
#include "ltr/crossvalidation/leave_one_out_splitter.h"
#include "ltr/crossvalidation/validation_result.h"
#include "ltr/scorers/fake_scorer.h"

using std::vector;

using ltr::FeatureInfo;
using ltr::Object;
using ltr::AbsError;
using ltr::BestFeatureLearner;
using ltr::cv::LeaveOneOutSplitter;
using ltr::cv::Validate;
using ltr::cv::ValidationResult;
using ltr::FakeScorer;
using ltr::PointwiseMeasure;

const int data_length = 11;
const FeatureInfo feature_info(1);

TEST(CrossvalidationTest, SimpleCrossvalidationTest) {
  DataSet<Object> data(feature_info);
  for (int object_index = 0; object_index < data_length; ++object_index) {
    Object object;
    object << 1;
    data.add(object);
  }
  FakeScorer::Ptr fake_scorer(new FakeScorer());
  fake_scorer->predict(data);

  AbsError::Ptr abs_measure_error(new AbsError);
  BestFeatureLearner<Object>::Ptr best_feature_learner(
    new BestFeatureLearner<Object>(abs_measure_error));

  vector<PointwiseMeasure::Ptr> abs_measure_vector;
  abs_measure_vector.push_back(abs_measure_error);

  LeaveOneOutSplitter<Object>::Ptr loo_splitter(new LeaveOneOutSplitter<Object>);

  ValidationResult vr = Validate(data,
                                 abs_measure_vector, 
                                 best_feature_learner, 
                                 loo_splitter);

  EXPECT_EQ(data_length, vr.getSplitCount());
  EXPECT_EQ(1, vr.getMeasureValues(0).size());
  EXPECT_EQ(1, vr.getMeasureNames().size());
  EXPECT_EQ(abs_measure_error->alias(), vr.getMeasureNames().at(0));

  for (int split = 0; split < vr.getSplitCount(); ++split) {
    Object test_obj;
    test_obj << 1;
    EXPECT_EQ(1, vr.getScorer(split)->score(test_obj));
    EXPECT_EQ(0, vr.getMeasureValues(split).at(0));
  }
};
