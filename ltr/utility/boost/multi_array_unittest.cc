// Copyright 2012 Yandex

#include <gtest/gtest.h>

#include "ltr/utility/boost/multi_array.h"

using ltr::utility::MultiArray;


TEST(MultiArrayTest, MultiArrayTest) {
  vector<int> dimensions;
  dimensions.push_back(6);
  dimensions.push_back(2);
  dimensions.push_back(4);
  MultiArray<int, 3> array(dimensions);
  EXPECT_EQ(array.shape(), dimensions);
  EXPECT_EQ(array.dimensions_count(), 3);

  vector<int> new_dimensions;
  new_dimensions.push_back(7);
  new_dimensions.push_back(1);
  new_dimensions.push_back(3);
  array.resize(new_dimensions);
  EXPECT_EQ(array.shape(), new_dimensions);

  vector<int> multi_index;
  multi_index.push_back(0);
  multi_index.push_back(0);
  multi_index.push_back(0);
  array(multi_index) = 10;
  EXPECT_EQ(array(multi_index), 10);
  EXPECT_EQ(array[0][0][0], 10);


}
