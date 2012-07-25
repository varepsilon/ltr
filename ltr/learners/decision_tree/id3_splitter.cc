// Copyright 2012 Yandex

#include <algorithm>

#include "ltr/learners/decision_tree/id3_splitter.h"
#include "ltr/utility/numerical.h"
#include "ltr/data/utility/io_utility.h"

#include "ltr/learners/decision_tree/compare_condition.h"

using ltr::utility::DoubleEqual;
using ltr::utility::DoubleLessOrEqual;

namespace ltr {
namespace decision_tree {

void ID3_Splitter::init() {
  INFO("Starting to init ID3_Splitter.");
  current_feature = 0;
  split_index = 0;
  INFO("Inited. ");
  if (split_feature_n_times_) {
    INFO("Splitting every feature %d times",
         feature_split_count_);
  } else {
    INFO("Using half summs splitting. Step = %d",
         half_summs_step_);
  }
}

int ID3_Splitter::getNextConditions(vector<Condition::Ptr>* result) {
  if (current_feature >= data_.feature_count()) {
    INFO("Current_feature is out of range.");
    return 0;
  }
  result->clear();

  if (split_index == 0) {
    INFO("Split_index is equal to zero.");
    feature_values.clear();
    numeric_split_values.clear();
    feature_values.resize(data_.size());

    double min_val = 1e9, max_val = -1e9;
    for (int j = 0; j < data_.size(); j++) {
      double feature =
        feature_values[j] = data_[j][current_feature];
      min_val = std::min(min_val, feature);
      max_val = std::max(max_val, feature);
    }
    std::sort(feature_values.begin(), feature_values.end());
    feature_values.resize(std::unique(feature_values.begin(),
                   feature_values.end(), DoubleEqual) - feature_values.begin());
    if (feature_values.size() <= 1) {
      INFO("Number of features is less or equal than one.");
      current_feature++;
      split_index = 0;
      return getNextConditions(result);
    }

    if (split_feature_n_times_) {
      int split_cnt = feature_split_count_;
      if (split_cnt <= feature_values.size() - 1) {
        for (int i = 0; i < split_cnt; i++) {
          numeric_split_values.
            push_back(min_val + (max_val - min_val) / split_cnt * i);
        }
      } else {
        for (int i = 0; i < feature_values.size() - 1; i++) {
          numeric_split_values.
            push_back((feature_values[i] + feature_values[i+1]) / 2);
        }
      }
    } else {
      for (int i = 0; i < feature_values.size() - 1;
          i+= half_summs_step_) {
        numeric_split_values.
          push_back((feature_values[i] + feature_values[i+1]) / 2);
      }
    }
  }
  if (data_.feature_info().getFeatureType(current_feature) == BOOLEAN ||
      data_.feature_info().getFeatureType(current_feature) == NOMINAL) {
    for (int j = 0; j < feature_values.size(); j++)
      result->push_back(
        CompareConditionPtr(OneFeatureConditionPtr(current_feature),
                            EQUAL, feature_values[j]));
    current_feature++;
    split_index = 0;
  } else {
    double split_val = numeric_split_values[split_index];
    result->push_back(
      CompareConditionPtr(
        OneFeatureConditionPtr(current_feature), LESS_OR_EQUAL, split_val));
    result->push_back(
      CompareConditionPtr(
        OneFeatureConditionPtr(current_feature), GREATER, split_val));
    split_index++;
    if (split_index >= numeric_split_values.size()) {
      split_index = 0;
      current_feature++;
    }
  }
  return 1;
}
}
}
