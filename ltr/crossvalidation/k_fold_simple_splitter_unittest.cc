// Copyright 2011 Yandex

#include <gtest/gtest.h>

#include <vector>

#include "ltr/crossvalidation/splitter.h"
#include "ltr/crossvalidation/splitter_unittest.h"
#include "ltr/crossvalidation/k_fold_simple_splitter.h"
#include "ltr/data/data_set.h"
#include "ltr/data/object.h"

using ltr::cv::KFoldSimpleSplitter;
using ltr::cv::SplittedDataSet;
using ltr::Object;

using std::vector;

TEST_F(SplitterTest, KFoldSimpleSplitterTest) {
  int k = 3;
  KFoldSimpleSplitter<Object> spl(k);
  EXPECT_EQ(k, spl.splitCount(data));

  vector<bool> used(data.size(), false);
  vector<int> test_sizes;

  for (int i = 0; i < spl.splitCount(data); ++i) {
    SplittedDataSet<Object> spl_data = spl.split(i, data);

    EXPECT_EQ(data.size(), spl_data.test_set.size() + spl_data.train_set.size());

    test_sizes.push_back(spl_data.test_set.size());

    for (int test_i = 0; test_i < spl_data.test_set.size(); ++test_i) {
      int test_object_feature = spl_data.test_set.at(test_i).features().at(0);
      EXPECT_FALSE(used[test_object_feature])  << ::testing::PrintToString(test_object_feature);
      used[test_object_feature] = true;
    }
  }

  for (int i = 0; i < used.size(); ++i) {
    EXPECT_TRUE(used[i]);
  }
  int diff = *std::max_element(test_sizes.begin(), test_sizes.end()) -
    *std::min_element(test_sizes.begin(), test_sizes.end());
  EXPECT_LE(diff, 1);
};
