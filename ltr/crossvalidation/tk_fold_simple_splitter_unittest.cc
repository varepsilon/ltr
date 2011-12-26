// Copyright 2011 Yandex

#include <gtest/gtest.h>

#include <vector>
#include <map>

#include "ltr/crossvalidation/splitter.h"
#include "ltr/crossvalidation/splitter_unittest.h"
#include "ltr/crossvalidation/tk_fold_simple_splitter.h"
#include "ltr/data/data_set.h"
#include "ltr/data/object.h"

using ltr::cv::TKFoldSimpleSplitter;
using ltr::cv::SplittedDataSet;
using ltr::Object;

using std::vector;
using std::map;
using std::pair;

TEST_F(SplitterTest, TKFoldSimpleSplitterAsKFoldTest) {
  int k = 3;
  int t = 4;
  TKFoldSimpleSplitter<Object> spl(k, t);
  EXPECT_EQ(t * k, spl.splitCount(data));

  // testing tkfold as kfold for each t
  for (int current_t = 0; current_t < t; ++current_t) {
    vector<bool> used(data.size(), false);
    vector<int> test_sizes;
    for (int block_i = 0; block_i < k; ++block_i) {
      SplittedDataSet<Object> spl_data =
        spl.split(current_t * k + block_i, data);
      EXPECT_EQ(data.size(), spl_data.test_set.size()
        + spl_data.train_set.size());

      test_sizes.push_back(spl_data.test_set.size());

      for (int test_i = 0; test_i < spl_data.test_set.size(); ++test_i) {
        int test_object_feature =
          spl_data.test_set.at(test_i).features().at(0);
        EXPECT_FALSE(used[test_object_feature])
          << ::testing::PrintToString(test_object_feature);
        used[test_object_feature] = true;
      }
    }

    for (int i = 0; i < used.size(); ++i) {
      EXPECT_TRUE(used[i]);
    }
    int diff = *std::max_element(test_sizes.begin(), test_sizes.end()) -
        *std::min_element(test_sizes.begin(), test_sizes.end());
    EXPECT_LE(diff, 1);
  }
};

bool Equal(const vector<int>& partition1, const vector<int>& partition2) {
  map<int, int> accordance;
  for (int i = 0; i < partition1.size(); ++i) {
    map<int, int>::iterator found = accordance.find(partition1[i]);
    if (found == accordance.end()) {
      accordance.insert(pair<int, int>(partition1[i], partition2[i]));
    } else {
      if (found->second != partition2[i]) {
        return false;
      }
    }
  }
  return true;
}

TEST_F(SplitterTest, TKFoldSimpleSplitterTUniquenessTest) {
  int k = 3;
  int t = 4;
  TKFoldSimpleSplitter<Object> spl(k, t);

  vector< vector<int> > marks(t);

  for (int current_t = 0; current_t < t; ++current_t) {
    marks[current_t].resize(data.size(), -1);

    for (int block_i = 0; block_i < k; ++block_i) {
      SplittedDataSet<Object> spl_data =
        spl.split(current_t * k + block_i, data);

      for (int test_i = 0; test_i < spl_data.test_set.size(); ++test_i) {
        int test_object_feature = spl_data.test_set.at(test_i).features().at(0);
        marks[current_t][test_object_feature] = block_i;
      }
    }
  }

  for (int first = 0; first < t; ++first) {
    for (int second = first + 1; second < t; ++second) {
      EXPECT_FALSE(Equal(marks[first], marks[second]));
    }
  }
};
