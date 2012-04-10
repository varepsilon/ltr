// Copyright 2012 Yandex

#include <algorithm>

#include "ltr/learners/decision_tree/id3_splitter.h"
#include "ltr/utility/numerical.h"
#include "ltr/data/utility/data_set_utility.h"

#include "ltr/learners/decision_tree/compare_condition.h"

using ltr::utility::DoubleEqual;
using ltr::utility::DoubleLessOrEqual;
using ltr::utility::lightSubset;

namespace ltr {
namespace decision_tree {

void ID3_Splitter::init() {
  current_feature = 0;
  split_idx = 0;
  log << "Inited. ";
  if (this->getBoolParameter("SPLIT_FEATURE_N_TIMES"))
    log << "Splitting every feature "
        << this->getIntParameter("FEATURE_SPLIT_COUNT") << " times"
        << std::endl;
  else
    log << "Using half summs splitting. Step = "
        << this->getIntParameter("HALF_SUMMS_STEP");
}

int ID3_Splitter::getNextConditions(vector<Condition::Ptr>* result) {
  if (current_feature >= data_.featureInfo().getFeatureCount())
    return 0;
  result->clear();

  if (split_idx == 0) {
//    log << "Splitting feature " << current_feature << std::endl;
    feature_values.clear();
    numeric_split_values.clear();
    feature_values.resize(data_.size());

    double min_val = 1e9, max_val = -1e9;
    for (int j = 0; j < data_.size(); j++) {
      double feature =
        feature_values[j] = data_[j].features()[current_feature];
      min_val = std::min(min_val, feature);
      max_val = std::max(max_val, feature);
    }
    std::sort(feature_values.begin(), feature_values.end());
    feature_values.resize(std::unique(feature_values.begin(),
                   feature_values.end(), DoubleEqual) - feature_values.begin());
    if (feature_values.size() <= 1) {
      current_feature++;
      split_idx = 0;
      return getNextConditions(result);
    }

    if (this->getBoolParameter("SPLIT_FEATURE_N_TIMES")) {
      int split_cnt = this->getIntParameter("FEATURE_SPLIT_COUNT");
      if (split_cnt <= feature_values.size() - 1) {
        for (int i = 0; i < split_cnt; i++)
          numeric_split_values.
            push_back(min_val + (max_val - min_val) / split_cnt * i);
      } else {
        for (int i = 0; i < feature_values.size() - 1; i++)
          numeric_split_values.
            push_back((feature_values[i] + feature_values[i+1]) / 2);
      }
    } else {
      for (int i = 0; i < feature_values.size() - 1;
          i+= this->getIntParameter("HALF_SUMMS_STEP"))
        numeric_split_values.
          push_back((feature_values[i] + feature_values[i+1]) / 2);
    }
  }
  if (data_.featureInfo().getFeatureType(current_feature) == BOOLEAN ||
      data_.featureInfo().getFeatureType(current_feature) == NOMINAL) {
    for (int j = 0; j < feature_values.size(); j++)
      result->push_back(
        CompareConditionPtr(OneFeatureConditionPtr(current_feature),
                            EQUAL, feature_values[j]));
    current_feature++;
    split_idx = 0;
  } else {
    double split_val = numeric_split_values[split_idx];
    result->push_back(
      CompareConditionPtr(
        OneFeatureConditionPtr(current_feature), LESS_OR_EQUAL, split_val));
    result->push_back(
      CompareConditionPtr(
        OneFeatureConditionPtr(current_feature), GREATER, split_val));
    split_idx++;
    if (split_idx >= numeric_split_values.size()) {
      split_idx = 0;
      current_feature++;
    }
  }
  return 1;
}
}
}
