// Copyright 2012 Yandex

#ifndef LTR_DECISION_TREE_FAKE_CONDITION_H_
#define LTR_DECISION_TREE_FAKE_CONDITION_H_

#include "ltr/decision_tree/decision_tree.h"
#include "ltr/scorers/fake_scorer.h"

namespace ltr {
namespace decision_tree {

typedef FakeScorer FakeCondition;

FakeCondition::Ptr FakeConditionPtr() {
  return FakeCondition::Ptr(new FakeCondition);
}
}
}

#endif  // LTR_DECISION_TREE_FAKE_CONDITION_H_
