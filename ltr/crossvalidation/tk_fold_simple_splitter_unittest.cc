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
using ltr::ParametersContainer;

using std::vector;
using std::map;
using std::pair;

const int k = 3;
const int t = 4;

TKFoldSimpleSplitter<Object> Use_k_t(int in_k, int in_t) {
  ParametersContainer param;
  param.setInt("K", in_k);
  param.setInt("T", in_t);
  return TKFoldSimpleSplitter<Object>(param);
}


TEST_F(SplitterTest, TKFoldSimpleSplitterAsKFoldTest) {
  TKFoldSimpleSplitter<Object> spl = Use_k_t(k, t);
  EXPECT_EQ(t * k, spl.splitCount(data));

  // testing tkfold as kfold for each t
  for (int current_t = 0; current_t < t; ++current_t) {
    vector<bool> used(data.size(), false);
    vector<int> test_sizes;
    // for every split of a kfold
    for (int block_i = 0; block_i < k; ++block_i) {
      SplittedDataSet<Object> spl_data =
        spl.split(current_t * k + block_i, data);
      EXPECT_EQ(data.size(), spl_data.test_set.size()
        + spl_data.train_set.size());

      // holding test sizes for checking if they are close
      test_sizes.push_back(spl_data.test_set.size());

      // every element expects not to be used in two different test data
      // (for different splits)
      for (int test_i = 0; test_i < spl_data.test_set.size(); ++test_i) {
        int test_object_feature =
          spl_data.test_set.at(test_i).features().at(0);
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
  }

  EXPECT_ANY_THROW(TKFoldSimpleSplitter<Object> spl1 = Use_k_t(1, t));
  EXPECT_ANY_THROW(TKFoldSimpleSplitter<Object> spl2 = Use_k_t(k, 0));
};

// partition is a mapping from element to part's number
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
  TKFoldSimpleSplitter<Object> spl = Use_k_t(k, t);

  vector< vector<int> > marks(t);

  // making t partitions
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

  // checking if there is no equal partitions
  for (int first = 0; first < t; ++first) {
    for (int second = first + 1; second < t; ++second) {
      EXPECT_FALSE(Equal(marks[first], marks[second]));
    }
  }
};
