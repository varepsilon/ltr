// Copyright 2011 Yandex

#include <gtest/gtest.h>

#include "ltr/data/data_set.h"
#include "ltr/data/object.h"
#include "ltr/data/feature_info.h"
#include "ltr/utility/numerical.h"
#include "ltr/parameters_container/parameters_container.h"

#include "ltr/learners/composition_learner/composition_learner.h"
#include "ltr/predictions_aggregators/sum_predictions_aggregator.h"
#include "ltr/predictions_aggregators/max_weight_predictions_aggregator.h"
#include "ltr/predictions_aggregators/order_statistic_predictions_aggregator.h"
#include "ltr/learners/best_feature_learner/best_feature_learner.h"
#include "ltr/measures/abs_error.h"
#include "ltr/measures/true_point.h"

using ltr::Object;
using ltr::DataSet;
using ltr::FeatureInfo;
using ltr::utility::DoubleEqual;
using ltr::ParametersContainer;

using ltr::composition::CompositionLearner;
using ltr::BestFeatureLearner;
using ltr::AbsError;
using ltr::TruePoint;
using ltr::SumPredictionsAggregator;
using ltr::MaxWeightPredictionsAggregator;
using ltr::OrderStatisticPredictionsAggregator;

const int data_size = 11;

class CompositionLearnerTest : public ::testing::Test {
  public:
  CompositionLearnerTest()
    : data(FeatureInfo(4)) {}
  protected:
  virtual void SetUp() {
    for (int i = 0; i < data_size; ++i) {
      Object object;
      object << i << data_size - i << i * i << i * (data_size - i);
      object.set_actual_label(i);
      object.set_predicted_label(data_size - i);
      data.add(object);
    }
  }

  DataSet<Object> data;
};

TEST_F(CompositionLearnerTest, LinearCompositionLearnerTest) {
  CompositionLearner<Object>
    linear_composition_learner(9);
  linear_composition_learner.setDefaultScorer();

  AbsError::Ptr abs_error(new AbsError);

  BestFeatureLearner<Object>::Ptr
    best_feature_learner(new BestFeatureLearner<Object>(abs_error));
  linear_composition_learner.set_weak_learner(best_feature_learner);

  linear_composition_learner.learn(data);
  CompositionScorer::Ptr linear_composition_scorer =
    linear_composition_learner.makeSpecific();

  EXPECT_EQ(9, linear_composition_scorer->size());
  for (int i = 0; i < linear_composition_scorer->size(); ++i) {
    linear_composition_scorer->at(i).scorer->predict(data);
    EXPECT_TRUE(DoubleEqual(0.0, abs_error->average(data)))
      << abs_error->average(data);
  }
}

TEST_F(CompositionLearnerTest, MaxWeightCompositionLearnerTest) {
  MaxWeightPredictionsAggregator::Ptr aggregator(
    new MaxWeightPredictionsAggregator);
  CompositionScorer scorer(aggregator);
  CompositionLearner<Object>
    composition_learner(9);
  composition_learner.setInitialScorer(scorer);

  AbsError::Ptr abs_error(new AbsError);

  BestFeatureLearner<Object>::Ptr
    best_feature_learner(new BestFeatureLearner<Object>(abs_error));
  composition_learner.set_weak_learner(best_feature_learner);

  composition_learner.learn(data);
  CompositionScorer::Ptr composition_scorer =
    composition_learner.makeSpecific();

  EXPECT_EQ(9, composition_scorer->size());
  for (int i = 0; i < composition_scorer->size(); ++i) {
    composition_scorer->at(i).scorer->predict(data);
    EXPECT_TRUE(DoubleEqual(0.0, abs_error->average(data)))
      << abs_error->average(data);
  }
}

TEST_F(CompositionLearnerTest, MedianCompositionLearnerTest) {
  OrderStatisticPredictionsAggregator::Ptr
    aggregator(new OrderStatisticPredictionsAggregator);
  CompositionScorer scorer(aggregator);
  CompositionLearner<Object>
    composition_learner(9);
  composition_learner.setInitialScorer(scorer);

  AbsError::Ptr abs_error(new AbsError);

  BestFeatureLearner<Object>::Ptr
    best_feature_learner(new BestFeatureLearner<Object>(abs_error));
  composition_learner.set_weak_learner(best_feature_learner);

  composition_learner.learn(data);
  CompositionScorer::Ptr composition_scorer =
    composition_learner.makeSpecific();

  EXPECT_EQ(9, composition_scorer->size());
  for (int i = 0; i < composition_scorer->size(); ++i) {
    composition_scorer->at(i).scorer->predict(data);
    EXPECT_TRUE(DoubleEqual(0.0, abs_error->average(data)))
      << abs_error->average(data);
  }
}

TEST_F(CompositionLearnerTest, OrderStaticticCompositionLearnerTest) {
  OrderStatisticPredictionsAggregator::Ptr aggregator(
    new OrderStatisticPredictionsAggregator(0.3));
  CompositionLearner<Object>
    composition_learner(9);
  composition_learner.setInitialScorer(CompositionScorer(aggregator));

  AbsError::Ptr abs_error(new AbsError);

  BestFeatureLearner<Object>::Ptr
    best_feature_learner(new BestFeatureLearner<Object>(abs_error));
  composition_learner.set_weak_learner(best_feature_learner);

  composition_learner.learn(data);
  CompositionScorer::Ptr composition_scorer =
    composition_learner.makeSpecific();

  EXPECT_EQ(9, composition_scorer->size());
  for (int i = 0; i < composition_scorer->size(); ++i) {
    composition_scorer->at(i).scorer->predict(data);
    EXPECT_TRUE(DoubleEqual(0.0, abs_error->average(data)))
      << abs_error->average(data);
  }
}
