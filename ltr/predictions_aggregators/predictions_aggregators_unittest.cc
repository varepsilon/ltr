// copyright 2012 Yandex

#include "gtest/gtest.h"

#include "ltr/predictions_aggregators/predictions_aggregator.h"
#include "ltr/predictions_aggregators/average_predictions_aggregator.h"
#include "ltr/predictions_aggregators/sum_predictions_aggregator.h"
#include "ltr/predictions_aggregators/vote_predictions_aggregator.h"
#include "ltr/utility/numerical.h"

using ltr::PredictionsAggregator;
using ltr::AveragePredictionsAggregator;
using ltr::SumPredictionsAggregator;
using ltr::VotePredictionsAggregator;
using ltr::utility::DoubleEqual;

TEST(PredictionsAggregators, PredictionsAggregatorsTest) {
  PredictionsAggregator::Ptr average_predictions_aggregator =
    new AveragePredictionsAggregator;

  vector<double> labels;
  labels.push_back(1.0);
  labels.push_back(2.0);
  labels.push_back(3.0);

  vector<double> weights;
  weights.push_back(0.2);
  weights.push_back(0.7);
  weights.push_back(0.1);

  EXPECT_TRUE(DoubleEqual(
    (1.0 * 0.2 + 2.0 * 0.7 + 3.0 * 0.1) / (0.2 + 0.7 + 0.1),
    average_predictions_aggregator->aggregate(labels, weights)));

  PredictionsAggregator::Ptr sum_predictions_aggregator =
    new SumPredictionsAggregator;
  EXPECT_TRUE(DoubleEqual(
    1.0 * 0.2 + 2.0 * 0.7 + 3.0 * 0.1,
    sum_predictions_aggregator->aggregate(labels, weights)));

  PredictionsAggregator::Ptr vote_predictions_aggregator =
    new VotePredictionsAggregator;

  labels.push_back(3.0);
  weights.push_back(1.0);
  EXPECT_TRUE(DoubleEqual(3.0,
    vote_predictions_aggregator->aggregate(labels, weights)));
}
