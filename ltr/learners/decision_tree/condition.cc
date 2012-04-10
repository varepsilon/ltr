// Copyright 2012 Yandex

#include "ltr/learners/decision_tree/condition.h"

namespace ltr {
namespace decision_tree {

FakeCondition::Ptr FakeConditionPtr() {
  return FakeCondition::Ptr(new FakeCondition);
}

OneFeatureCondition::Ptr OneFeatureConditionPtr() {
  return OneFeatureCondition::Ptr(new OneFeatureCondition());
}

OneFeatureCondition::Ptr OneFeatureConditionPtr(int feature_id) {
  return OneFeatureCondition::Ptr(new OneFeatureCondition(feature_id));
}
}
}
