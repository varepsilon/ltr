// Copyright 2012 Yandex

#include <vector>
#include <gtest/gtest.h>

#include "ltr/utility/numerical.h"
#include "ltr/data/object.h"
#include "ltr/scorers/composition_scorers/linear_composition_scorer.h"
#include "ltr/scorers/composition_scorers/max_weight_composition_scorer.h"
#include "ltr/scorers/composition_scorers/median_composition_scorer.h"
#include "ltr/scorers/composition_scorers/order_statistic_composition_scorer.h"
#include "ltr/scorers/one_feature_scorer.h"

using ltr::Object;
using ltr::composition::LinearCompositionScorer;
using ltr::composition::MaxWeightCompositionScorer;
using ltr::composition::MedianCompositionScorer;
using ltr::composition::OrderStatisticCompositionScorer;
using ltr::OneFeatureScorer;
using ltr::utility::DoubleEqual;
using ltr::ParametersContainer;
using std::vector;

TEST(CompositionScorer, LinearCompositionScorerTest) {
  Object object;
  object << 1 << 2 << 3;

  OneFeatureScorer::Ptr one_feature_scorer0(new OneFeatureScorer(0));
  OneFeatureScorer::Ptr one_feature_scorer1(new OneFeatureScorer(1));
  OneFeatureScorer::Ptr one_feature_scorer2(new OneFeatureScorer(2));

  EXPECT_TRUE(DoubleEqual(one_feature_scorer0->score(object), 1));
  EXPECT_TRUE(DoubleEqual(one_feature_scorer1->score(object), 2));
  EXPECT_TRUE(DoubleEqual(one_feature_scorer2->score(object), 3));

  LinearCompositionScorer::Ptr linear_composition_scorer(new LinearCompositionScorer);
  linear_composition_scorer->add(one_feature_scorer0, 0.5);
  EXPECT_TRUE(DoubleEqual(linear_composition_scorer->score(object), 0.5));
  linear_composition_scorer->add(one_feature_scorer1, 1);
  EXPECT_TRUE(DoubleEqual(linear_composition_scorer->score(object), 2.5));
  linear_composition_scorer->add(one_feature_scorer2, 1.5);
  EXPECT_TRUE(DoubleEqual(linear_composition_scorer->score(object), 7));
  linear_composition_scorer->clear();
  EXPECT_TRUE(DoubleEqual(linear_composition_scorer->score(object), 0));
  linear_composition_scorer->add(one_feature_scorer0, 2);
  EXPECT_TRUE(DoubleEqual(linear_composition_scorer->score(object), 2));
}

TEST(CompositionScorer, MaxWeightCompositionScorerTest) {
  Object object;
  object << 1 << 2 << 3;

  OneFeatureScorer::Ptr one_feature_scorer0(new OneFeatureScorer(0));
  OneFeatureScorer::Ptr one_feature_scorer1(new OneFeatureScorer(1));
  OneFeatureScorer::Ptr one_feature_scorer2(new OneFeatureScorer(2));

  MaxWeightCompositionScorer::Ptr max_weight_composition_scorer(new MaxWeightCompositionScorer);
  EXPECT_ANY_THROW(max_weight_composition_scorer->score(object));
  max_weight_composition_scorer->add(one_feature_scorer0, 0.5);
  EXPECT_TRUE(DoubleEqual(max_weight_composition_scorer->score(object), 1));
  max_weight_composition_scorer->add(one_feature_scorer1, 0.3);
  EXPECT_TRUE(DoubleEqual(max_weight_composition_scorer->score(object), 1));
  max_weight_composition_scorer->add(one_feature_scorer2, 1);
  EXPECT_TRUE(DoubleEqual(max_weight_composition_scorer->score(object), 3));
  max_weight_composition_scorer->add(one_feature_scorer1, 1.5);
  EXPECT_TRUE(DoubleEqual(max_weight_composition_scorer->score(object), 2));
  max_weight_composition_scorer->clear();
  max_weight_composition_scorer->add(one_feature_scorer2, 1);
  EXPECT_TRUE(DoubleEqual(max_weight_composition_scorer->score(object), 3));
  max_weight_composition_scorer->add(one_feature_scorer0, 2);
  EXPECT_TRUE(DoubleEqual(max_weight_composition_scorer->score(object), 1));
}

TEST(CompositionScorer, MedianCompositionScorerTest) {
  Object object;
  object << 1 << 2 << 3 << 4 << 5;

  OneFeatureScorer::Ptr one_feature_scorer0(new OneFeatureScorer(0));
  OneFeatureScorer::Ptr one_feature_scorer1(new OneFeatureScorer(1));
  OneFeatureScorer::Ptr one_feature_scorer2(new OneFeatureScorer(2));
  OneFeatureScorer::Ptr one_feature_scorer3(new OneFeatureScorer(3));
  OneFeatureScorer::Ptr one_feature_scorer4(new OneFeatureScorer(4));

  MedianCompositionScorer::Ptr median_composition_scorer(new MedianCompositionScorer);
  EXPECT_ANY_THROW(median_composition_scorer->score(object));
  median_composition_scorer->add(one_feature_scorer0);
  EXPECT_TRUE(DoubleEqual(median_composition_scorer->score(object), 1));
  median_composition_scorer->add(one_feature_scorer1);
  median_composition_scorer->add(one_feature_scorer4);
  EXPECT_TRUE(DoubleEqual(median_composition_scorer->score(object), 2));
  median_composition_scorer->add(one_feature_scorer3, 2);
  EXPECT_TRUE(DoubleEqual(median_composition_scorer->score(object), 4));
  median_composition_scorer->clear();
  median_composition_scorer->add(one_feature_scorer0, 2);
  median_composition_scorer->add(one_feature_scorer4);
  EXPECT_TRUE(DoubleEqual(median_composition_scorer->score(object), 1));
}

TEST(CompositionScorer, OrderStaticticCompositionScorerTest) {
  Object object;
  object << 1 << 2 << 3 << 4 << 5;

  OneFeatureScorer::Ptr one_feature_scorer0(new OneFeatureScorer(0));
  OneFeatureScorer::Ptr one_feature_scorer1(new OneFeatureScorer(1));
  OneFeatureScorer::Ptr one_feature_scorer2(new OneFeatureScorer(2));
  OneFeatureScorer::Ptr one_feature_scorer3(new OneFeatureScorer(3));
  OneFeatureScorer::Ptr one_feature_scorer4(new OneFeatureScorer(4));

  ParametersContainer parameters_container;
  parameters_container.AddNew("RATIO", 0.3);
  OrderStatisticCompositionScorer::Ptr order_statistic_composition_scorer
    (new OrderStatisticCompositionScorer(parameters_container));
  EXPECT_ANY_THROW(order_statistic_composition_scorer->score(object));
  order_statistic_composition_scorer->add(one_feature_scorer0);
  EXPECT_TRUE(DoubleEqual(order_statistic_composition_scorer->score(object), 1));
  order_statistic_composition_scorer->add(one_feature_scorer1);
  order_statistic_composition_scorer->add(one_feature_scorer4);
  EXPECT_TRUE(DoubleEqual(order_statistic_composition_scorer->score(object), 1));
  order_statistic_composition_scorer->add(one_feature_scorer3);
  EXPECT_TRUE(DoubleEqual(order_statistic_composition_scorer->score(object), 2));
  order_statistic_composition_scorer->add(one_feature_scorer0);
  EXPECT_TRUE(DoubleEqual(order_statistic_composition_scorer->score(object), 1));
}
