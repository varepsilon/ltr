// Copyright 2012 Yandex

#ifndef LTR_DECISION_TREE_FAKE_CONDITION_H_
#define LTR_DECISION_TREE_FAKE_CONDITION_H_

#include "ltr/decision_tree/decision_tree.h"

namespace ltr {
namespace DecisionTree {

class FakeCondition : public Condition {
  public:
    typedef boost::shared_ptr<FakeCondition> Ptr;

  private:
    double valueImpl(const Object& obj) const;
};

FakeCondition::Ptr FakeConditionPtr();
}
}

#endif  // LTR_DECISION_TREE_FAKE_CONDITION_H_
