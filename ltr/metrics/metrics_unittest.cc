// Copyright 2011 Yandex School Practice

#include <gtest/gtest.h>

#include "ltr/data/object.h"
#include "ltr/metrics/metric.h"
#include "ltr/metrics/manhattan_metric.h"
#include "ltr/metrics/euclidean_metric.h"

#include "ltr/utility/numerical.h"

using ltr::Object;
using ltr::utility::DoubleEqual;

TEST(MetricTest, TestEuclideanMetric) {
  Object object1;
  Object object2;

  object1 << 1.0;
  ltr::EuclideanMetric euclidean_metric;
  EXPECT_ANY_THROW(euclidean_metric.distance(object1, object2));

  object2 << 4.0;
  EXPECT_TRUE(DoubleEqual(euclidean_metric.distance(object1, object2),
                          3.0));

  object1 << -0.7;
  object2 << -4.7;
  EXPECT_TRUE(DoubleEqual(euclidean_metric.distance(object1, object2), 5.0));
}

TEST(MetricTest, TestManhattanMetric) {
  Object object1;
  Object object2;

  object1 << 15.15;
  ltr::ManhattanMetric manhattan_metric;
  EXPECT_ANY_THROW(manhattan_metric.distance(object1, object2));

  object2 << -3.75;
  EXPECT_TRUE(DoubleEqual(15.15 + 3.75,
                          manhattan_metric.distance(object1, object2)));

  object1 << 1.0 << 0.0;
  object2 << 3.9 << -12.4;
  EXPECT_TRUE(DoubleEqual((15.15 + 3.75) + 2.9 + 12.4,
                          manhattan_metric.distance(object1, object2)));
}
