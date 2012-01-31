// Copyright 2011 Yandex

#include <gtest/gtest.h>
#include <vector>

#include "ltr/crossvalidation/crossvalidation.h"
#include "ltr/learners/best_feature_learner.h"
#include "ltr/data/object.h"
#include "ltr/measures/abs_error.h"
#include "ltr/crossvalidation/leave_one_out_splitter.h"
#include "ltr/crossvalidation/validation_result.h"
#include "ltr/scorers/utility/scorer_utility.h"
#include "ltr/scorers/fake_scorer.h"

using std::vector;

using ltr::Object;
using ltr::AbsError;
using ltr::BestFeatureLearner;
using ltr::cv::LeaveOneOutSplitter;
using ltr::cv::Validate;
using ltr::cv::ValidationResult;
using ltr::FakeScorer;
using ltr::utility::MarkDataSet;

TEST(CrossvalidationTest, CompilingCrossvalidationTest) {
  DataSet<Object> data;
  int data_lenght = 11;
  for (int i = 0; i < data_lenght; ++i) {
    Object obj;
    obj << i;
    data.add(obj);
  }
  FakeScorer::Ptr fscorer(new FakeScorer());
  MarkDataSet(data, *fscorer);

  AbsError::Ptr ab_measure(new AbsError);
  BestFeatureLearner<Object>::Ptr bfl(
    new BestFeatureLearner<Object>(ab_measure));

  vector<AbsError> abm_vector;
  abm_vector.push_back(*ab_measure);
  LeaveOneOutSplitter<Object> spl;

  // not works yet
  // ValidationResult vr(Validate(data, abm_vector, *bfl, spl));
};
