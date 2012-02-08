// Copyright 2011 Yandex

#include <gtest/gtest.h>
#include <vector>

#include "ltr/crossvalidation/crossvalidation.h"
#include "ltr/learners/best_feature_learner.h"
#include "ltr/data/object.h"
#include "ltr/data/feature_info.h"
#include "ltr/measures/abs_error.h"
#include "ltr/measures/measure.h"
#include "ltr/crossvalidation/leave_one_out_splitter.h"
#include "ltr/crossvalidation/validation_result.h"
#include "ltr/scorers/utility/scorer_utility.h"
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
using ltr::utility::MarkDataSet;
using ltr::PointwiseMeasure;

const int data_lenght = 11;
const FeatureInfo fi(1);

TEST(CrossvalidationTest, SimpleCrossvalidationTest) {
  DataSet<Object> data(fi);
  for (int i = 0; i < data_lenght; ++i) {
    Object obj;
    obj << 1;
    data.add(obj);
  }
  FakeScorer::Ptr fscorer(new FakeScorer());
  MarkDataSet(data, *fscorer);

  AbsError::Ptr ab_measure(new AbsError);
  BestFeatureLearner<Object>::Ptr bfl(
    new BestFeatureLearner<Object>(ab_measure));

  vector<PointwiseMeasure::Ptr> abm_vector;
  abm_vector.push_back(ab_measure);

  LeaveOneOutSplitter<Object>::Ptr spl(new LeaveOneOutSplitter<Object>);

  ValidationResult vr = Validate(data, abm_vector, bfl, spl);

  EXPECT_EQ(data_lenght, vr.getSplitCount());
  EXPECT_EQ(1, vr.getMeasureValues(0).size());
  EXPECT_EQ(1, vr.getMeasureNames().size());
  string expected_str = "Absolute error";
  EXPECT_EQ(expected_str, vr.getMeasureNames().at(0));

  for (int split = 0; split < vr.getSplitCount(); ++split) {
    Object test_obj;
    test_obj << 1;
    EXPECT_EQ(1, vr.getScorer(split)->score(test_obj));
    EXPECT_EQ(0, vr.getMeasureValues(split).at(0));
  }
};
