// Copyright 2012 Yandex

#include "ltr/decision_tree/fake_condition.h"

namespace ltr {
namespace DecisionTree {
  double FakeCondition::valueImpl(const Object& obj) const {
    return 0.0;
  }

  FakeCondition::Ptr FakeConditionPtr() {
    return FakeCondition::Ptr(new FakeCondition);
  }
}
}
