// copyright 2012 Yandex

#ifndef LTR_DECISION_TREE_ONE_FEATURE_CONDITION_H_
#define LTR_DECISION_TREE_ONE_FEATURE_CONDITION_H_

#include "ltr/decision_tree/decision_tree.h"
#include "ltr/scorers/one_feature_scorer.h"

namespace ltr {
namespace decision_tree {

typedef OneFeatureScorer OneFeatureCondition;

OneFeatureCondition::Ptr OneFeatureConditionPtr() {
  return OneFeatureCondition::Ptr(new OneFeatureCondition());
}

OneFeatureCondition::Ptr OneFeatureConditionPtr(int feature_id) {
  return OneFeatureCondition::Ptr(new OneFeatureCondition(feature_id));
}
}
}

#endif  // LTR_DECISION_TREE_ONE_FEATURE_CONDITION_H_
