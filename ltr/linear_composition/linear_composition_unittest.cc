// Copyright 2011 Yandex

#include <gtest/gtest.h>

#include "ltr/data/data_set.h"
#include "ltr/data/object.h"
#include "ltr/data/feature_info.h"
#include "ltr/scorers/utility/scorer_utility.h"
#include "ltr/utility/numerical.h"

#include "ltr/linear_composition/data_set_weights_updater.h"
#include "ltr/linear_composition/linear_composition_scorer_weights_updater.h"

#include "ltr/linear_composition/linear_composition_learner.h"
#include "ltr/linear_composition/ada_rank_data_set_weights_updater.h"
#include "ltr/linear_composition/ada_rank_lc_scorer_weights_updater.h"
#include "ltr/linear_composition/average_lc_scorer_weights_updater.h"

#include "ltr/learners/best_feature_learner.h"
#include "ltr/measures/abs_error.h"

using ltr::Object;
using ltr::DataSet;
using ltr::FeatureInfo;
using ltr::LinearCompositionScorer;
using ltr::utility::MarkDataSet;
using ltr::utility::DoubleEqual;

using ltr::BestFeatureLearner;
using ltr::lc::LinearCompositionLearner;
using ltr::AbsError;
using ltr::lc::AverageLCScorerWeightsUpdater;

const int data_size = 11;

class LinearCompositionTest : public ::testing::Test {
  public:
  LinearCompositionTest()
    : data(FeatureInfo(2)) {}
  protected:
  virtual void SetUp() {
    for (int i = 0; i < data_size; ++i) {
      Object obj;
      obj << i << data_size - i << i * i;
      obj.setActualLabel(i);
      obj.setPredictedLabel(data_size - i);
      data.add(obj);
    }
  }

  DataSet<Object> data;
};

TEST_F(LinearCompositionTest, SimpleLinearCompositionTest) {
  LinearCompositionLearner<Object> lc_learner;

  AbsError::Ptr abs_error(new AbsError);
  lc_learner.setMeasure(abs_error);

  BestFeatureLearner<Object>::Ptr bf_learner(new BestFeatureLearner<Object>);
  lc_learner.setWeakLearner(bf_learner);

  lc_learner.learn(data);
  LinearCompositionScorer lin_scorer = lc_learner.make();

  EXPECT_EQ(10, lin_scorer.size());
  for (int i = 0; i < lin_scorer.size(); ++i) {
    MarkDataSet(data, *lin_scorer[i].scorer);
    EXPECT_TRUE(DoubleEqual(0.0, abs_error->average(data)))
      << abs_error->average(data);
  }

  LinearCompositionLearner<Object,
    AverageLCScorerWeightsUpdater> av_lc_learner;
  av_lc_learner.setMeasure(abs_error);
  av_lc_learner.setWeakLearner(bf_learner);
  av_lc_learner.learn(data);
  LinearCompositionScorer av_lin_scorer = av_lc_learner.make();
  MarkDataSet(data, av_lin_scorer);
  EXPECT_NEAR(0.0, abs_error->average(data), 1e-8);
}
