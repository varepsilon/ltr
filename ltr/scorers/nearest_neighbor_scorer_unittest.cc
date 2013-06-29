// Copyright 2012 Yandex

#include <gtest/gtest.h>
#include <vector>

#include "ltr/predictions_aggregators/predictions_aggregator.h"
#include "ltr/predictions_aggregators/average_predictions_aggregator.h"
#include "ltr/predictions_aggregators/vote_predictions_aggregator.h"
#include "ltr/data/object.h"
#include "ltr/data/data_set.h"
#include "ltr/utility/numerical.h"
#include "ltr/scorers/nearest_neighbor_scorer.h"
#include "ltr/metrics/euclidean_metric.h"
#include "ltr/utility/neighbor_weighter.h"

using std::vector;

using ltr::PredictionsAggregator;
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

  NNScorer::Ptr nn_scorer(
    new NNScorer(new ltr::EuclideanMetric,
                 data,
                 new ltr::utility::InverseLinearDistance,
                 new ltr::AveragePredictionsAggregator,
                 2));

  Object score_object;
  score_object << 0.0 << 0.0 << 0.0;

  EXPECT_TRUE(DoubleEqual(4. / 3., (*nn_scorer)(score_object)));
}
