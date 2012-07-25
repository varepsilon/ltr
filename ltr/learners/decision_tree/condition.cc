// Copyright 2012 Yandex

#include <logog/logog.h>

#include "ltr/learners/decision_tree/condition.h"

namespace ltr {
namespace decision_tree {

FakeCondition::Ptr FakeConditionPtr() {
  INFO("Creating pointer to FakeCondition.");
  return FakeCondition::Ptr(new FakeCondition);
}

OneFeatureCondition::Ptr OneFeatureConditionPtr() {
  INFO("Creating pointer to OneFeatureCondition.");
  return OneFeatureCondition::Ptr(new OneFeatureCondition());
}

OneFeatureCondition::Ptr OneFeatureConditionPtr(int feature_id) {
  INFO("Creating pointer to OneFeatureCondition with feature_id equal to %d.",
    feature_id);
  return OneFeatureCondition::Ptr(new OneFeatureCondition(feature_id));
}
}
}
