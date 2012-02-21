// Copyright 2012 Yandex

#include <algorithm>

#include "ltr/learners/utility/id3_splitter.h"
#include "ltr/utility/numerical.h"
#include "ltr/data/utility/data_set_utility.h"

#include "ltr/decision_tree/compare_condition.h"

using ltr::utility::DoubleEqual;
using ltr::utility::DoubleLessOrEqual;
using ltr::utility::lightSubset;

namespace ltr {
namespace decision_tree {

void ID3_Splitter::init() {
  current_feature = 0;
  split_idx = 0;
}

int ID3_Splitter::getNextConditions(vector<Condition::Ptr>* result) {
  if (current_feature >= data_.featureInfo().getFeatureCount())
    return 0;
  result->clear();

  if (split_idx == 0) {
    feature_values.clear();
    feature_values.resize(data_.size());

    for (int j = 0; j < data_.size(); j++)
        feature_values[j] = data_[j].features()[current_feature];
    std::sort(feature_values.begin(), feature_values.end());
    feature_values.resize(std::unique(feature_values.begin(),
                   feature_values.end(), DoubleEqual) - feature_values.begin());
    if (feature_values.size() <= 1) {
      current_feature++;
      split_idx = 0;
      return getNextConditions(result);
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
    double split_val = (feature_values[split_idx] +
                        feature_values[split_idx + 1]) / 2;
    result->push_back(
      CompareConditionPtr(OneFeatureConditionPtr(current_feature),
                          LESS_OR_EQUAL, split_val));
    result->push_back(
      CompareConditionPtr(OneFeatureConditionPtr(current_feature),
                          GREATER, split_val));
    split_idx++;
    if (split_idx + 1 >= feature_values.size()) {
      split_idx = 0;
      current_feature++;
    }
  }
  return 1;
}
}
}
