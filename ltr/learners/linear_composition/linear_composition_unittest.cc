// Copyright 2011 Yandex

#include <gtest/gtest.h>

#include "ltr/data/data_set.h"
#include "ltr/data/object.h"
#include "ltr/data/feature_info.h"
#include "ltr/scorers/utility/scorer_utility.h"
#include "ltr/utility/numerical.h"

#include "ltr/learners/linear_composition/data_set_weights_updater.h"
#include "ltr/learners/linear_composition/linear_composition_scorer_weights_updater.h"

#include "ltr/learners/linear_composition/linear_composition_learner.h"
#include "ltr/learners/linear_composition/ada_rank_data_set_weights_updater.h"
#include "ltr/learners/linear_composition/ada_rank_lc_scorer_weights_updater.h"
#include "ltr/learners/linear_composition/average_lc_scorer_weights_updater.h"

#include "ltr/learners/best_feature_learner/best_feature_learner.h"
#include "ltr/measures/abs_error.h"
#include "ltr/measures/true_point.h"
#include "ltr/data_preprocessors/data_random_sampler.h"
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
using ltr::BeggingPreprocessor;
using ltr::BestFeatureLearner;
using ltr::lc::LinearCompositionLearner;
using ltr::AbsError;
using ltr::lc::AverageLCScorerWeightsUpdater;
using ltr::TruePoint;

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

  BestFeatureLearner<Object>::Ptr bf_learner(new BestFeatureLearner<Object>);
  BeggingPreprocessor<Object>::Ptr begging(new BeggingPreprocessor<Object>);
  bf_learner->addDataPreprocessor(begging);
  lc_learner.setWeakLearner(bf_learner);

  lc_learner.learn(data);
  LinearCompositionScorer lin_scorer = lc_learner.make();

  EXPECT_EQ(10, lin_scorer.size());
  for (int i = 0; i < lin_scorer.size(); ++i) {
    MarkDataSet(data, *lin_scorer[i].scorer);
    EXPECT_TRUE(DoubleEqual(0.0, abs_error->average(data)))
      << abs_error->average(data);
  }

  begging->setExistingParameter("SELECTED_PART", 2.);
  lc_learner.setExistingParameter("NUMBER_OF_ITERATIONS", 15);
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

  BestFeatureLearner<Object>::Ptr bf_learner(new BestFeatureLearner<Object>);
  bf_learner->addFeatureConverter(rsm);
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

  TruePoint::Ptr true_point(new TruePoint);
  adalcs_lc_learner.setMeasure(true_point);

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

  TruePoint::Ptr true_point(new TruePoint);
  ada_lc_learner.setMeasure(true_point);

  BestFeatureLearner<Object>::Ptr bf_learner(new BestFeatureLearner<Object>);
  ada_lc_learner.setWeakLearner(bf_learner);

  RSMFeatureConverterLearner<Object>::Ptr
    rsm(new RSMFeatureConverterLearner<Object>);
  bf_learner->addFeatureConverter(rsm);

  BeggingPreprocessor<Object>::Ptr begging(new BeggingPreprocessor<Object>);
  bf_learner->addDataPreprocessor(begging);

  ada_lc_learner.learn(data);
  LinearCompositionScorer lin_scorer = ada_lc_learner.make();

  EXPECT_NO_THROW(MarkDataSet(data, lin_scorer));
}
