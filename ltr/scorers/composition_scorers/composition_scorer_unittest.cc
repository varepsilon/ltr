// Copyright 2012 Yandex

#include <gtest/gtest.h>

#include <vector>

#include "ltr/utility/numerical.h"
#include "ltr/data/object.h"
#include "ltr/scorers/one_feature_scorer.h"
#include "ltr/scorers/composition_scorers/composition_scorer.h"
#include "ltr/predictions_aggregators/sum_predictions_aggregator.h"
#include "ltr/predictions_aggregators/max_weight_predictions_aggregator.h"
#include "ltr/predictions_aggregators/order_statistic_predictions_aggregator.h"

using ltr::Object;
using ltr::composition::CompositionScorer;
using ltr::SumPredictionsAggregator;
using ltr::MaxWeightPredictionsAggregator;
using ltr::OrderStatisticPredictionsAggregator;
using ltr::OneFeatureScorer;
using ltr::utility::DoubleEqual;
using ltr::ParametersContainer;
using std::vector;

TEST(CompositionScorer, CompositionScorerWithSumAggregationTest) {
  Object object;
  object << 1 << 2 << 3;

  OneFeatureScorer::Ptr one_feature_scorer0(new OneFeatureScorer(0));
  OneFeatureScorer::Ptr one_feature_scorer1(new OneFeatureScorer(1));
  OneFeatureScorer::Ptr one_feature_scorer2(new OneFeatureScorer(2));

  EXPECT_TRUE(DoubleEqual(one_feature_scorer0->score(object), 1));
  EXPECT_TRUE(DoubleEqual(one_feature_scorer1->score(object), 2));
  EXPECT_TRUE(DoubleEqual(one_feature_scorer2->score(object), 3));

  SumPredictionsAggregator::Ptr aggregator(new SumPredictionsAggregator);
  CompositionScorer::Ptr composition_scorer(new CompositionScorer(aggregator));
  composition_scorer->add(one_feature_scorer0, 0.5);
  EXPECT_TRUE(DoubleEqual(composition_scorer->score(object), 0.5));
  composition_scorer->add(one_feature_scorer1, 1);
  EXPECT_TRUE(DoubleEqual(composition_scorer->score(object), 2.5));
  composition_scorer->add(one_feature_scorer2, 1.5);
  EXPECT_TRUE(DoubleEqual(composition_scorer->score(object), 7));
  composition_scorer->clear();
  EXPECT_TRUE(DoubleEqual(composition_scorer->score(object), 0));
  composition_scorer->add(one_feature_scorer0, 2);
  EXPECT_TRUE(DoubleEqual(composition_scorer->score(object), 2));
}

TEST(CompositionScorer, CompositionScorerWithMaxWeightAggregationTest) {
  Object object;
  object << 1 << 2 << 3;

  OneFeatureScorer::Ptr one_feature_scorer0(new OneFeatureScorer(0));
  OneFeatureScorer::Ptr one_feature_scorer1(new OneFeatureScorer(1));
  OneFeatureScorer::Ptr one_feature_scorer2(new OneFeatureScorer(2));

  MaxWeightPredictionsAggregator::Ptr aggregator(
    new MaxWeightPredictionsAggregator);
  CompositionScorer::Ptr composition_scorer(new CompositionScorer(aggregator));
  EXPECT_ANY_THROW(composition_scorer->score(object));
  composition_scorer->add(one_feature_scorer0, 0.5);
  EXPECT_TRUE(DoubleEqual(composition_scorer->score(object), 1));
  composition_scorer->add(one_feature_scorer1, 0.3);
  EXPECT_TRUE(DoubleEqual(composition_scorer->score(object), 1));
  composition_scorer->add(one_feature_scorer2, 1);
  EXPECT_TRUE(DoubleEqual(composition_scorer->score(object), 3));
  composition_scorer->add(one_feature_scorer1, 1.5);
  EXPECT_TRUE(DoubleEqual(composition_scorer->score(object), 2));
  composition_scorer->clear();
  composition_scorer->add(one_feature_scorer2, 1);
  EXPECT_TRUE(DoubleEqual(composition_scorer->score(object), 3));
  composition_scorer->add(one_feature_scorer0, 2);
  EXPECT_TRUE(DoubleEqual(composition_scorer->score(object), 1));
}


TEST(CompositionScorer, CompositionScorerWithMedianAggregationTest) {
  Object object;
  object << 1 << 2 << 3 << 4 << 5;

  OneFeatureScorer::Ptr one_feature_scorer0(new OneFeatureScorer(0));
  OneFeatureScorer::Ptr one_feature_scorer1(new OneFeatureScorer(1));
  OneFeatureScorer::Ptr one_feature_scorer2(new OneFeatureScorer(2));
  OneFeatureScorer::Ptr one_feature_scorer3(new OneFeatureScorer(3));
  OneFeatureScorer::Ptr one_feature_scorer4(new OneFeatureScorer(4));

  OrderStatisticPredictionsAggregator::Ptr aggregator(
    new OrderStatisticPredictionsAggregator());
  CompositionScorer::Ptr composition_scorer(new CompositionScorer(aggregator));
  EXPECT_ANY_THROW(composition_scorer->score(object));
  composition_scorer->add(one_feature_scorer0);
  EXPECT_TRUE(DoubleEqual(composition_scorer->score(object), 1));
  composition_scorer->add(one_feature_scorer1);
  composition_scorer->add(one_feature_scorer4);
  EXPECT_TRUE(DoubleEqual(composition_scorer->score(object), 2));
  composition_scorer->add(one_feature_scorer3, 2);
  EXPECT_TRUE(DoubleEqual(composition_scorer->score(object), 4));
  composition_scorer->clear();
  composition_scorer->add(one_feature_scorer0, 2);
  composition_scorer->add(one_feature_scorer4);
  EXPECT_TRUE(DoubleEqual(composition_scorer->score(object), 1));
}

TEST(CompositionScorer, CompositionScorerWithOrderStatisticAggregationTest) {
  Object object;
  object << 1 << 2 << 3 << 4 << 5;

  OneFeatureScorer::Ptr one_feature_scorer0(new OneFeatureScorer(0));
  OneFeatureScorer::Ptr one_feature_scorer1(new OneFeatureScorer(1));
  OneFeatureScorer::Ptr one_feature_scorer2(new OneFeatureScorer(2));
  OneFeatureScorer::Ptr one_feature_scorer3(new OneFeatureScorer(3));
  OneFeatureScorer::Ptr one_feature_scorer4(new OneFeatureScorer(4));

  OrderStatisticPredictionsAggregator::Ptr aggregator(
    new OrderStatisticPredictionsAggregator(0.3));
  CompositionScorer::Ptr composition_scorer
    (new CompositionScorer(aggregator));
  EXPECT_ANY_THROW(composition_scorer->score(object));
  composition_scorer->add(one_feature_scorer0);
  EXPECT_TRUE(DoubleEqual(composition_scorer->score(object), 1));
  composition_scorer->add(one_feature_scorer1);
  composition_scorer->add(one_feature_scorer4);
  EXPECT_TRUE(DoubleEqual(composition_scorer->score(object), 1));
  composition_scorer->add(one_feature_scorer3);
  EXPECT_TRUE(DoubleEqual(composition_scorer->score(object), 2));
  composition_scorer->add(one_feature_scorer0);
  EXPECT_TRUE(DoubleEqual(composition_scorer->score(object), 1));
}
