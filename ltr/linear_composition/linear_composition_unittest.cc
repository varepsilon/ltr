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
#include "ltr/data_preprocessors/begging_preprocessor_learner.h"
#include "ltr/feature_converters/RSM_feature_converter_learner.h"

using ltr::Object;
using ltr::DataSet;
using ltr::FeatureInfo;
using ltr::LinearCompositionScorer;
using ltr::utility::MarkDataSet;
using ltr::utility::DoubleEqual;

using ltr::lc::FakeDataSetWeightsUpdater;
using ltr::lc::FakeLCScorerWeightsUpdater;
using ltr::lc::AdaRankDataSetWeightsUpdater;
using ltr::lc::AdaRankLCScorerWeightsUpdater;
using ltr::RSMFeatureConverterLearner;
using ltr::BeggingPreprocessorLearner;
using ltr::BestFeatureLearner;
using ltr::lc::LinearCompositionLearner;
using ltr::AbsError;
using ltr::lc::AverageLCScorerWeightsUpdater;

const int data_size = 11;

class LinearCompositionTest : public ::testing::Test {
  public:
  LinearCompositionTest()
    : data(FeatureInfo(4)) {}
  protected:
  virtual void SetUp() {
    for (int i = 0; i < data_size; ++i) {
      Object obj;
      obj << i << data_size - i << i * i << i * (data_size - i);
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

  AverageLCScorerWeightsUpdater<Object>::Ptr
    av_lcswu(new AverageLCScorerWeightsUpdater<Object>);

  LinearCompositionLearner<Object> av_lc_learner;
  av_lc_learner.setLCScorerWeightsUpdater(av_lcswu);
  av_lc_learner.setMeasure(abs_error);
  av_lc_learner.setWeakLearner(bf_learner);
  av_lc_learner.learn(data);
  LinearCompositionScorer av_lin_scorer = av_lc_learner.make();
  MarkDataSet(data, av_lin_scorer);
  EXPECT_NEAR(0.0, abs_error->average(data), 1e-8);
}

TEST_F(LinearCompositionTest, BeggingSimpleLinearCompositionTest) {
  LinearCompositionLearner<Object> lc_learner;

  AbsError::Ptr abs_error(new AbsError);
  lc_learner.setMeasure(abs_error);

  BeggingPreprocessorLearner<Object>::Ptr
    begging(new BeggingPreprocessorLearner<Object>);
  lc_learner.setDataPreprocessorLearner(begging);

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

  begging->setDoubleParameter("SELECTED_PART", 2);
  lc_learner.setDataPreprocessorLearner(begging);
  lc_learner.setIntParameter("NUMBER_OF_ITERATIONS", 15);
  lc_learner.learn(data);
  lin_scorer = lc_learner.make();

  EXPECT_EQ(25, lin_scorer.size());
  for (int i = 0; i < lin_scorer.size(); ++i) {
    MarkDataSet(data, *lin_scorer[i].scorer);
    EXPECT_TRUE(DoubleEqual(0.0, abs_error->average(data)))
      << abs_error->average(data);
  }
}

TEST_F(LinearCompositionTest, RSMSimpleLinearCompositionTest) {
  LinearCompositionLearner<Object> lc_learner;

  AbsError::Ptr abs_error(new AbsError);
  lc_learner.setMeasure(abs_error);

  RSMFeatureConverterLearner<Object>::Ptr
    rsm(new RSMFeatureConverterLearner<Object>);
  lc_learner.setFeatureConverterLearner(rsm);

  BestFeatureLearner<Object>::Ptr bf_learner(new BestFeatureLearner<Object>);
  lc_learner.setWeakLearner(bf_learner);

  lc_learner.learn(data);
  LinearCompositionScorer lin_scorer = lc_learner.make();

  EXPECT_NO_THROW(MarkDataSet(data, lin_scorer));
}

TEST_F(LinearCompositionTest, AdaRankDSWUSimpleLinearCompositionTest) {
  LinearCompositionLearner<Object> adads_lc_learner;

  AdaRankDataSetWeightsUpdater<Object>::Ptr
    ada_dswu(new AdaRankDataSetWeightsUpdater<Object>);
  adads_lc_learner.setDataSetWeightsUpdater(ada_dswu);

  AbsError::Ptr abs_error(new AbsError);
  adads_lc_learner.setMeasure(abs_error);

  BestFeatureLearner<Object>::Ptr bf_learner(new BestFeatureLearner<Object>);
  adads_lc_learner.setWeakLearner(bf_learner);

  adads_lc_learner.learn(data);
  LinearCompositionScorer lin_scorer = adads_lc_learner.make();

  EXPECT_NO_THROW(MarkDataSet(data, lin_scorer));
}

TEST_F(LinearCompositionTest, AdaRankLCSWUSimpleLinearCompositionTest) {
  LinearCompositionLearner<Object> adalcs_lc_learner;

  AdaRankLCScorerWeightsUpdater<Object>::Ptr
    ada_lcswu(new AdaRankLCScorerWeightsUpdater<Object>);
  adalcs_lc_learner.setLCScorerWeightsUpdater(ada_lcswu);
  // careful here: abs_error is not in [0..1] whereas
  // AdaRankLCScorerWeightsUpdater requires it
  // new version in progress
  AbsError::Ptr abs_error(new AbsError);
  adalcs_lc_learner.setMeasure(abs_error);

  BestFeatureLearner<Object>::Ptr bf_learner(new BestFeatureLearner<Object>);
  adalcs_lc_learner.setWeakLearner(bf_learner);

  adalcs_lc_learner.learn(data);
  LinearCompositionScorer lin_scorer = adalcs_lc_learner.make();

  EXPECT_NO_THROW(MarkDataSet(data, lin_scorer));
}

TEST_F(LinearCompositionTest, AdaRankBeggingRSMSimpleLinearCompositionTest) {
  LinearCompositionLearner<Object> ada_lc_learner;

  AdaRankLCScorerWeightsUpdater<Object>::Ptr
    ada_lcswu(new AdaRankLCScorerWeightsUpdater<Object>);
  ada_lc_learner.setLCScorerWeightsUpdater(ada_lcswu);

  AdaRankDataSetWeightsUpdater<Object>::Ptr
    ada_dswu(new AdaRankDataSetWeightsUpdater<Object>);
  ada_lc_learner.setDataSetWeightsUpdater(ada_dswu);
  // careful here: abs_error is not in [0..1] whereas
  // AdaRankLCScorerWeightsUpdater requires it
  // new version in progress
  AbsError::Ptr abs_error(new AbsError);
  ada_lc_learner.setMeasure(abs_error);

  BestFeatureLearner<Object>::Ptr bf_learner(new BestFeatureLearner<Object>);
  ada_lc_learner.setWeakLearner(bf_learner);

  RSMFeatureConverterLearner<Object>::Ptr
    rsm(new RSMFeatureConverterLearner<Object>);
  ada_lc_learner.setFeatureConverterLearner(rsm);

  BeggingPreprocessorLearner<Object>::Ptr
    begging(new BeggingPreprocessorLearner<Object>);
  ada_lc_learner.setDataPreprocessorLearner(begging);

  ada_lc_learner.learn(data);
  LinearCompositionScorer lin_scorer = ada_lc_learner.make();

  EXPECT_NO_THROW(MarkDataSet(data, lin_scorer));
}
