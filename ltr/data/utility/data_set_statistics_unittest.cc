// Copyright 2013 Yandex

#include <cstdlib>
#include <vector>

#include "gtest/gtest.h"

#include "ltr/data/utility/data_set_statistics.h"

using std::vector;

using ltr::DataSet;
using ltr::utility::getFeaturesMinMaxValues;
using ltr::utility::DoubleEqual;

TEST(DataSetStatisticsTest, TestingGetFeaturesMinMaxValues) {
  DataSet<Object> data;
  Object object1, object2, object3, object4;
  object1 << 1.0 << -10.0 << 0.0;
  object2 << 7.0 << -8.0 << 30.0;
  object3 << 13.0 << -0.5 << -30.0;
  object4 << 8.0 << -3.0 << 0.0;
  data.add(object1);
  data.add(object2);
  data.add(object3);
  data.add(object4);

  vector<double> min_values, max_values;
  getFeaturesMinMaxValues(data, &min_values, &max_values);

  EXPECT_TRUE(DoubleEqual(min_values[0], 1.0));
  EXPECT_TRUE(DoubleEqual(min_values[1], -10.0));
  EXPECT_TRUE(DoubleEqual(min_values[2], -30.0));

  EXPECT_TRUE(DoubleEqual(max_values[0], 13.0));
  EXPECT_TRUE(DoubleEqual(max_values[1], -0.5));
  EXPECT_TRUE(DoubleEqual(max_values[2], 30.0));
}
