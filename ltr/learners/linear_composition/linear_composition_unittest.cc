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
#include "ltr/feature_converters/feature_random_sampler_learner.h"

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
using ltr::FeatureRandomSamplerLearner;
using ltr::DataRandomSampler;
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
      obj.set_actual_label(i);
      obj.set_predicted_label(data_size - i);
      data.add(obj);
    }
  }

  DataSet<Object> data;
};

TEST_F(LinearCompositionTest, SimpleLinearCompositionTest) {
  LinearCompositionLearner<Object> lc_learner;

  AbsError::Ptr abs_error(new AbsError);
  lc_learner.set_measure(abs_error);

  BestFeatureLearner<Object>::Ptr bf_learner(new BestFeatureLearner<Object>);
  lc_learner.set_weak_learner(bf_learner);

  lc_learner.learn(data);
  LinearCompositionScorer::Ptr lin_scorer = lc_learner.makeSpecific();

  EXPECT_EQ(10, lin_scorer->size());
  for (int i = 0; i < lin_scorer->size(); ++i) {
    MarkDataSet(data, *(lin_scorer->at(i).scorer));
    EXPECT_TRUE(DoubleEqual(0.0, abs_error->average(data)))
      << abs_error->average(data);
  }

  AverageLCScorerWeightsUpdater<Object>::Ptr
    av_lcswu(new AverageLCScorerWeightsUpdater<Object>);

  LinearCompositionLearner<Object> av_lc_learner;
  av_lc_learner.setLCScorerWeightsUpdater(av_lcswu);
  av_lc_learner.set_measure(abs_error);
  av_lc_learner.set_weak_learner(bf_learner);
  av_lc_learner.learn(data);
  LinearCompositionScorer::Ptr av_lin_scorer = av_lc_learner.makeSpecific();
  MarkDataSet(data, *av_lin_scorer);
  EXPECT_NEAR(0.0, abs_error->average(data), 1e-8);
}

TEST_F(LinearCompositionTest, BeggingSimpleLinearCompositionTest) {
  LinearCompositionLearner<Object> lc_learner;

  AbsError::Ptr abs_error(new AbsError);
  lc_learner.set_measure(abs_error);

  BestFeatureLearner<Object>::Ptr bf_learner(new BestFeatureLearner<Object>);
  DataRandomSampler<Object>::Ptr begging(new DataRandomSampler<Object>);
  bf_learner->addDataPreprocessor(begging);
  lc_learner.set_weak_learner(bf_learner);

  lc_learner.learn(data);
  LinearCompositionScorer::Ptr lin_scorer = lc_learner.makeSpecific();

  EXPECT_EQ(10, lin_scorer->size());
  for (int i = 0; i < lin_scorer->size(); ++i) {
    MarkDataSet(data, *(lin_scorer->at(i).scorer));
    EXPECT_TRUE(DoubleEqual(0.0, abs_error->average(data)))
      << abs_error->average(data);
  }

  begging->set_sampling_fraction(2.);
  lc_learner.setExistingParameter("NUMBER_OF_ITERATIONS", 15);
  lc_learner.learn(data);
  lin_scorer = lc_learner.makeSpecific();

  EXPECT_EQ(25, lin_scorer->size());
  for (int i = 0; i < lin_scorer->size(); ++i) {
    MarkDataSet(data, *(lin_scorer->at(i).scorer));
    EXPECT_TRUE(DoubleEqual(0.0, abs_error->average(data)))
      << abs_error->average(data);
  }
}

TEST_F(LinearCompositionTest, RSMSimpleLinearCompositionTest) {
  LinearCompositionLearner<Object> lc_learner;

  AbsError::Ptr abs_error(new AbsError);
  lc_learner.set_measure(abs_error);

  FeatureRandomSamplerLearner<Object>::Ptr
    rsm(new FeatureRandomSamplerLearner<Object>);

  BestFeatureLearner<Object>::Ptr bf_learner(new BestFeatureLearner<Object>);
  bf_learner->addFeatureConverterLearner(rsm);
  lc_learner.set_weak_learner(bf_learner);

  lc_learner.learn(data);
  LinearCompositionScorer::Ptr lin_scorer = lc_learner.makeSpecific();

  EXPECT_NO_THROW(MarkDataSet(data, *lin_scorer));
}

TEST_F(LinearCompositionTest, AdaRankDSWUSimpleLinearCompositionTest) {
  LinearCompositionLearner<Object> adads_lc_learner;

  AdaRankDataSetWeightsUpdater<Object>::Ptr
    ada_dswu(new AdaRankDataSetWeightsUpdater<Object>);
  adads_lc_learner.setDataSetWeightsUpdater(ada_dswu);

  AbsError::Ptr abs_error(new AbsError);
  adads_lc_learner.set_measure(abs_error);

  BestFeatureLearner<Object>::Ptr bf_learner(new BestFeatureLearner<Object>);
  adads_lc_learner.set_weak_learner(bf_learner);

  adads_lc_learner.learn(data);
  LinearCompositionScorer::Ptr lin_scorer = adads_lc_learner.makeSpecific();

  EXPECT_NO_THROW(MarkDataSet(data, *lin_scorer));
}

TEST_F(LinearCompositionTest, AdaRankLCSWUSimpleLinearCompositionTest) {
  LinearCompositionLearner<Object> adalcs_lc_learner;

  AdaRankLCScorerWeightsUpdater<Object>::Ptr
    ada_lcswu(new AdaRankLCScorerWeightsUpdater<Object>);
  adalcs_lc_learner.setLCScorerWeightsUpdater(ada_lcswu);

  TruePoint::Ptr true_point(new TruePoint);
  adalcs_lc_learner.set_measure(true_point);

  BestFeatureLearner<Object>::Ptr bf_learner(new BestFeatureLearner<Object>);
  adalcs_lc_learner.set_weak_learner(bf_learner);

  adalcs_lc_learner.learn(data);
  LinearCompositionScorer::Ptr lin_scorer = adalcs_lc_learner.makeSpecific();

  EXPECT_NO_THROW(MarkDataSet(data, *lin_scorer));
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
  ada_lc_learner.set_measure(true_point);

  BestFeatureLearner<Object>::Ptr bf_learner(new BestFeatureLearner<Object>);
  ada_lc_learner.set_weak_learner(bf_learner);

  FeatureRandomSamplerLearner<Object>::Ptr
    rsm(new FeatureRandomSamplerLearner<Object>);
  bf_learner->addFeatureConverterLearner(rsm);

  DataRandomSampler<Object>::Ptr begging(new DataRandomSampler<Object>);
  bf_learner->addDataPreprocessor(begging);

  ada_lc_learner.learn(data);
  LinearCompositionScorer::Ptr lin_scorer = ada_lc_learner.makeSpecific();

  EXPECT_NO_THROW(MarkDataSet(data, *lin_scorer));
}
