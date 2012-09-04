// Copyright 2012 Yandex

#include <gtest/gtest.h>
#include <vector>

#include "ltr/aggregators/aggregator.h"
#include "ltr/aggregators/average_aggregator.h"
#include "ltr/aggregators/vote_aggregator.h"
#include "ltr/data/object.h"
#include "ltr/data/data_set.h"
#include "ltr/utility/numerical.h"
#include "ltr/scorers/nearest_neighbor_scorer.h"
#include "ltr/metrics/euclidean_metric.h"
#include "ltr/utility/neighbor_weighter.h"

using std::vector;

using ltr::Aggregator;
using ltr::DataSet;
using ltr::Object;
using ltr::NNScorer;
using ltr::utility::DoubleEqual;
using ltr::utility::NeighborWeighter;

TEST(NNScorer, NNScorerTest) {
  Object object1;
  Object object2;
  Object object3;

  object1 << 1.0 << 0.0 << 0.0;
  object2 << 0.0 << 0.0 << 3.0;
  object3 << 0.0 << 2.0 << 0.0;

  object1.set_actual_label(1.0);
  object2.set_actual_label(3.0);
  object3.set_actual_label(2.0);

  DataSet<Object> data;
  data.add(object1);
  data.add(object2);
  data.add(object3);

  data.setWeight(0, 1);
  data.setWeight(1, 1);
  data.setWeight(2, 1);

  NNScorer nn_scorer(new ltr::EuclideanMetric,
                     data,
                     new ltr::utility::InverseLinearDistance,
                     new ltr::AverageAggregator,
                     2);

  Object score_object;
  score_object << 0.0 << 0.0 << 0.0;

  EXPECT_TRUE(DoubleEqual(1.0, nn_scorer(score_object)));
}
