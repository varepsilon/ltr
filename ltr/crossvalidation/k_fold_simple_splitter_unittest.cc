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
using ltr::ParametersContainer;

using std::vector;

const int k = 3;

KFoldSimpleSplitter<Object> Use_k(int in_k) {
  ParametersContainer param;
  param.Set("K", in_k);
  return KFoldSimpleSplitter<Object>(param);
}

TEST_F(SplitterTest, KFoldSimpleSplitterTest) {
  KFoldSimpleSplitter<Object> spl = Use_k(k);

  EXPECT_EQ(k, spl.splitCount(data));

  vector<bool> used(data.size(), false);
  vector<int> test_sizes;

  // for every split
  for (int i = 0; i < spl.splitCount(data); ++i) {
    SplittedDataSet<Object> spl_data = spl.split(i, data);

    EXPECT_EQ(data.size(), spl_data.test_set.size()
      + spl_data.train_set.size());
    // holding test sizes for checking if they are close
    test_sizes.push_back(spl_data.test_set.size());

    // every element expects not to be used in two different test data
    // (for different splits)
    for (int test_i = 0; test_i < spl_data.test_set.size(); ++test_i) {
      int test_object_feature = spl_data.test_set.at(test_i).features().at(0);
      EXPECT_FALSE(used[test_object_feature])
        << ::testing::PrintToString(test_object_feature);
      used[test_object_feature] = true;
    }
  }

  // every element expects being used in test data at least once
  for (int i = 0; i < used.size(); ++i) {
    EXPECT_TRUE(used[i]);
  }
  // checking if test data sizes are close for different splits
  int diff = *std::max_element(test_sizes.begin(), test_sizes.end()) -
    *std::min_element(test_sizes.begin(), test_sizes.end());
  EXPECT_LE(diff, 1);

  EXPECT_ANY_THROW(KFoldSimpleSplitter<Object> spl1 = Use_k(1));
};
