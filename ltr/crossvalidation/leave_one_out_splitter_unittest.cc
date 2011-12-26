// Copyright 2011 Yandex

#include <gtest/gtest.h>

#include <vector>

#include "ltr/crossvalidation/splitter.h"
#include "ltr/crossvalidation/splitter_unittest.h"
#include "ltr/crossvalidation/leave_one_out_splitter.h"
#include "ltr/data/data_set.h"
#include "ltr/data/object.h"

using ltr::cv::LeaveOneOutSplitter;
using ltr::cv::SplittedDataSet;
using ltr::Object;

using std::vector;

TEST_F(SplitterTest, LeaveOneOutSplitterTest) {
  LeaveOneOutSplitter<Object> spl;
  EXPECT_EQ(data.size(), spl.splitCount(data));

  vector<bool> used(data.size(), false);
  for (int i = 0; i < spl.splitCount(data); ++i) {
    SplittedDataSet<Object> spl_data = spl.split(i, data);
    EXPECT_EQ(1, spl_data.test_set.size());
    EXPECT_EQ(data.size() - 1, spl_data.train_set.size());

    int test_object_feature = spl_data.test_set.at(0).features().at(0);
    EXPECT_FALSE(used[test_object_feature]);
    used[test_object_feature] = true;
  }

  for (int i = 0; i < used.size(); ++i) {
    EXPECT_TRUE(used[i]);
  }
};
