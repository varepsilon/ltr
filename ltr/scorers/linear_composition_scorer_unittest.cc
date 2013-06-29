// Copyright 2012 Yandex

#include <gtest/gtest.h>
#include <vector>

#include "ltr/utility/numerical.h"
#include "ltr/data/object.h"
#include "ltr/scorers/composition_scorers/composition_scorer.h"
#include "ltr/scorers/linear_scorer.h"
#include "ltr/scorers/one_feature_scorer.h"
#include "ltr/predictions_aggregators/sum_predictions_aggregator.h"

using std::vector;

using ltr::Object;
using ltr::LinearScorer;
using ltr::composition::CompositionScorer;
using ltr::SumPredictionsAggregator;
using ltr::OneFeatureScorer;
using ltr::utility::DoubleEqual;

TEST(LinearCompositionScorer, LinearCompositionScorerTest) {
  vector<double> weights1(2);
  weights1[0] = 0.5;
  weights1[1] = 1.7;

  LinearScorer::Ptr linear_scorer1(new LinearScorer(weights1));
  OneFeatureScorer::Ptr one_feature_scorer1(new OneFeatureScorer());

  Object obj1;
  obj1 << 1.2;

  SumPredictionsAggregator::Ptr aggregator(new SumPredictionsAggregator);
  CompositionScorer::Ptr linear_composition_scorer1(
      new CompositionScorer(aggregator));
  linear_composition_scorer1->add(linear_scorer1, 5.2);
  linear_composition_scorer1->add(one_feature_scorer1, 4.2);

  EXPECT_TRUE(DoubleEqual(18.248, (*linear_composition_scorer1)(obj1)));

  vector<double> weights2(5);
  weights2[0] = 0.4;
  weights2[1] = 0.92;
  weights2[2] = 0.003;
  weights2[3] = 0.1;
  weights2[4] = 0.2;

  vector<double> weights3(5);
  weights3[0] = 1.2;
  weights3[1] = 1.7;
  weights3[2] = 1.9;
  weights3[3] = 0.02;
  weights3[4] = 0.07;

  LinearScorer::Ptr linear_scorer2(new LinearScorer(weights2));
  LinearScorer::Ptr linear_scorer3(new LinearScorer(weights3));

  CompositionScorer::Ptr linear_composition_scorer2(
      new CompositionScorer(aggregator));
  linear_composition_scorer2->add(linear_scorer2, 2.1);
  linear_composition_scorer2->add(linear_scorer3, 0.7);

  Object obj2;
  obj2 << 1.9 << 3.7 << 0.2 << 4.1;

  EXPECT_TRUE(DoubleEqual(14.52381, (*linear_composition_scorer2)(obj2)));
  EXPECT_EQ(2, linear_composition_scorer2->size());

  LinearScorer::Ptr linear_scorer4(new LinearScorer(weights2));
  linear_composition_scorer2->clear();
  linear_composition_scorer2->add(linear_scorer4, 1);
  EXPECT_TRUE(DoubleEqual(2.9991, (*linear_composition_scorer2)(obj2)));
  EXPECT_EQ(1, linear_composition_scorer2->size());

  linear_composition_scorer2->toString();
}
