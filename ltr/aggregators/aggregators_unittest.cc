// copyright 2012 Yandex

#include "gtest/gtest.h"

#include "ltr/aggregators/aggregator.h"
#include "ltr/aggregators/average_aggregator.h"
#include "ltr/aggregators/sum_aggregator.h"
#include "ltr/aggregators/vote_aggregator.h"
#include "ltr/utility/numerical.h"

using ltr::Aggregator;
using ltr::AverageAggregator;
using ltr::SumAggregator;
using ltr::VoteAggregator;
using ltr::utility::DoubleEqual;

TEST(Aggregators, AggregatorsTest) {
  Aggregator::Ptr average_aggregator = new AverageAggregator;

  vector<double> labels;
  labels.push_back(1.0);
  labels.push_back(2.0);
  labels.push_back(3.0);

  vector<double> weights;
  weights.push_back(0.2);
  weights.push_back(0.7);
  weights.push_back(0.1);

  EXPECT_TRUE(DoubleEqual((1.0 * 0.2 + 2.0 * 0.7 + 3.0 * 0.1) / 3.0,
                          average_aggregator->aggregate(labels, weights)));

  Aggregator::Ptr sum_aggregator = new SumAggregator;
  EXPECT_TRUE(DoubleEqual(1.0 * 0.2 + 2.0 * 0.7 + 3.0 * 0.1,
                          sum_aggregator->aggregate(labels, weights)));

  Aggregator::Ptr vote_aggregator = new VoteAggregator;

  labels.push_back(3.0);
  weights.push_back(1.0);
  EXPECT_TRUE(DoubleEqual(3.0, vote_aggregator->aggregate(labels, weights)));
}
