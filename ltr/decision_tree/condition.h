// Copyright 2012 Yandex

#ifndef LTR_DECISION_TREE_CONDITION_H_
#define LTR_DECISION_TREE_CONDITION_H_

#include "ltr/utility/serializable_functor.h"

#include "ltr/scorers/fake_scorer.h"
#include "ltr/scorers/one_feature_scorer.h"

namespace ltr {
namespace decision_tree {

typedef SerializableFunctor<double> Condition;

typedef FakeScorer FakeCondition;
typedef OneFeatureScorer OneFeatureCondition;

FakeCondition::Ptr FakeConditionPtr();
OneFeatureCondition::Ptr OneFeatureConditionPtr();
OneFeatureCondition::Ptr OneFeatureConditionPtr(int feature_id);
}
}
#endif  // LTR_DECISION_TREE_CONDITION_H_
