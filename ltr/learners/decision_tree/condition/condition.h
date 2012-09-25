// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_DECISION_TREE_CONDITION_CONDITION_H_
#define LTR_LEARNERS_DECISION_TREE_CONDITION_CONDITION_H_

#include "ltr/data/object.h"

#include "ltr/utility/shared_ptr.h"

namespace ltr {
namespace decision_tree {
/**
 * Condition is a base class for decision tree conditions.
 * It returns a value for tested Object.
 */
class Condition {
 public:
  typedef ltr::utility::shared_ptr<Condition> Ptr;

  virtual ~Condition() {}
  /*
   * Returns the confidence that object satisfied this condition.
   */
  virtual double value(const Object& object) const = 0;
};

class FakeCondition : public Condition {
 public:
  typedef ltr::utility::shared_ptr<FakeCondition> Ptr;

  FakeCondition() {}

  double value(const Object& object) const {return 1.0;}
};
};
};

#endif  // LTR_LEARNERS_DECISION_TREE_CONDITION_CONDITION_H_
