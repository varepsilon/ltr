// Copyright 2011 Yandex

#include <gtest/gtest.h>

#include "ltr/data/data_set.h"
#include "ltr/data/object.h"
#include "ltr/data/feature_info.h"
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
  LinearCompositionLearner<Object> lin_composition_learner;

  AbsError::Ptr abs_error(new AbsError);

  BestFeatureLearner<Object>::Ptr 
    best_feature_learner(new BestFeatureLearner<Object>(abs_error));
  lin_composition_learner.set_weak_learner(best_feature_learner);

  lin_composition_learner.learn(data);
  LinearCompositionScorer::Ptr lin_scorer = lin_composition_learner.makeSpecific();

  EXPECT_EQ(10, lin_scorer->size());
  for (int i = 0; i < lin_scorer->size(); ++i) {
    lin_scorer->at(i).scorer->markDataSet(data);
    EXPECT_TRUE(DoubleEqual(0.0, abs_error->average(data)))
      << abs_error->average(data);
  }

  AverageLCScorerWeightsUpdater<Object>::Ptr
    average_lin_composition_scorer_weights_updater(
      new AverageLCScorerWeightsUpdater<Object>);

  LinearCompositionLearner<Object> average_lin_composition_learner;
  average_lin_composition_learner.setLCScorerWeightsUpdater(
    average_lin_composition_scorer_weights_updater);  
  average_lin_composition_learner.set_weak_learner(best_feature_learner);
  average_lin_composition_learner.learn(data);
  LinearCompositionScorer::Ptr average_lin_scorer =
    average_lin_composition_learner.makeSpecific();
  average_lin_scorer->markDataSet(data);
  EXPECT_NEAR(0.0, abs_error->average(data), 1e-8);
}

TEST_F(LinearCompositionTest, BeggingSimpleLinearCompositionTest) {
  LinearCompositionLearner<Object> lin_composition_learner;

  AbsError::Ptr abs_error(new AbsError);  

  BestFeatureLearner<Object>::Ptr 
    best_feature_learner(new BestFeatureLearner<Object>(abs_error));
  DataRandomSampler<Object>::Ptr bagging(new DataRandomSampler<Object>);
  best_feature_learner->addDataPreprocessor(bagging);
  lin_composition_learner.set_weak_learner(best_feature_learner);

  lin_composition_learner.learn(data);
  LinearCompositionScorer::Ptr lin_scorer =
    lin_composition_learner.makeSpecific();

  EXPECT_EQ(10, lin_scorer->size());
  for (int i = 0; i < lin_scorer->size(); ++i) {
    lin_scorer->at(i).scorer->markDataSet(data);
    EXPECT_TRUE(DoubleEqual(0.0, abs_error->average(data)))
      << abs_error->average(data);
  }

  bagging->set_sampling_fraction(2.);
  lin_composition_learner.set_number_of_iterations(15);
  lin_composition_learner.learn(data);
  lin_scorer = lin_composition_learner.makeSpecific();

  EXPECT_EQ(25, lin_scorer->size());
  for (int i = 0; i < lin_scorer->size(); ++i) {
    lin_scorer->at(i).scorer->markDataSet(data);
    EXPECT_TRUE(DoubleEqual(0.0, abs_error->average(data)))
      << abs_error->average(data);
  }
}

TEST_F(LinearCompositionTest, RSMSimpleLinearCompositionTest) {
  LinearCompositionLearner<Object> lin_composition_learner;

  AbsError::Ptr abs_error(new AbsError);  

  FeatureRandomSamplerLearner<Object>::Ptr
    rsm(new FeatureRandomSamplerLearner<Object>);

  BestFeatureLearner<Object>::Ptr 
    best_feature_learner(new BestFeatureLearner<Object>(abs_error));
  best_feature_learner->addFeatureConverterLearner(rsm);
  lin_composition_learner.set_weak_learner(best_feature_learner);

  lin_composition_learner.learn(data);
  LinearCompositionScorer::Ptr lin_scorer =
    lin_composition_learner.makeSpecific();

  EXPECT_NO_THROW(lin_scorer->markDataSet(data));
}

TEST_F(LinearCompositionTest, AdaRankDSWUSimpleLinearCompositionTest) {
  LinearCompositionLearner<Object> ada_lin_composition_learner;

  AdaRankDataSetWeightsUpdater<Object>::Ptr
    ada_rank_data_set_weight_updater(
      new AdaRankDataSetWeightsUpdater<Object>);  

  AbsError::Ptr abs_error(new AbsError);  

  BestFeatureLearner<Object>::Ptr 
    best_feature_learner(new BestFeatureLearner<Object>(abs_error));
  ada_lin_composition_learner.set_weak_learner(best_feature_learner);

  ada_lin_composition_learner.learn(data);
  LinearCompositionScorer::Ptr lin_scorer =
    ada_lin_composition_learner.makeSpecific();

  EXPECT_NO_THROW(lin_scorer->markDataSet(data));
}

TEST_F(LinearCompositionTest, AdaRankLCSWUSimpleLinearCompositionTest) {
  LinearCompositionLearner<Object> ada_lin_composition_learner;

  AdaRankLCScorerWeightsUpdater<Object>::Ptr
    ada_rank_weight_updater(new AdaRankLCScorerWeightsUpdater<Object>);  

  TruePoint::Ptr true_point(new TruePoint);
  AbsError::Ptr abs_error(new AbsError);  

  BestFeatureLearner<Object>::Ptr 
    best_feature_learner(new BestFeatureLearner<Object>(abs_error));
  ada_lin_composition_learner.set_weak_learner(best_feature_learner);

  ada_lin_composition_learner.learn(data);
  LinearCompositionScorer::Ptr lin_scorer =
    ada_lin_composition_learner.makeSpecific();

  EXPECT_NO_THROW(lin_scorer->markDataSet(data));
}

TEST_F(LinearCompositionTest, AdaRankBeggingRSMSimpleLinearCompositionTest) {
  LinearCompositionLearner<Object> ada_learner;

  AdaRankLCScorerWeightsUpdater<Object>::Ptr
    ada_rank_weights_updater(new AdaRankLCScorerWeightsUpdater<Object>);  

  AdaRankDataSetWeightsUpdater<Object>::Ptr
    ada_rank_data_set_weights_updater(
      new AdaRankDataSetWeightsUpdater<Object>);  

  TruePoint::Ptr true_point(new TruePoint);
  AbsError::Ptr abs_error(new AbsError);  

  BestFeatureLearner<Object>::Ptr 
    best_feature_learner(new BestFeatureLearner<Object>(abs_error));
  ada_learner.set_weak_learner(best_feature_learner);

  FeatureRandomSamplerLearner<Object>::Ptr
    random_sampler_learner(new FeatureRandomSamplerLearner<Object>);
  best_feature_learner->addFeatureConverterLearner(
    random_sampler_learner);

  DataRandomSampler<Object>::Ptr bagging(new DataRandomSampler<Object>);
  best_feature_learner->addDataPreprocessor(bagging);

  ada_learner.learn(data);
  LinearCompositionScorer::Ptr lin_scorer = ada_learner.makeSpecific();

  EXPECT_NO_THROW(lin_scorer->markDataSet(data));
}
