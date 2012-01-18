// Copyright 2012 Yandex

#ifndef LTR_DECISION_TREE_COMPARE_CONDITION_H_
#define LTR_DECISION_TREE_COMPARE_CONDITION_H_

#include "ltr/decision_tree/decision_tree.h"

namespace ltr {
namespace DecisionTree {

enum CompareType {
  EQUAL,
  GREATER,
  LESS,
  GREATER_OR_EQUAL,
  LESS_OR_EQUAL,
};

class CompareCondition : public Condition {
  public:
    typedef boost::shared_ptr<CompareCondition> Ptr;

    void setCompareType(CompareType type);
    void setCompareNumber(double number);
    void setWeakCondition(Condition::Ptr weak_condition);

    CompareCondition() {}
    CompareCondition(Condition::Ptr weak_condition,
                     CompareType type,
                     double number) :
        weak_condition_(weak_condition),
        compare_type_(type),
        compare_number_(number) {}

  private:
    double valueImpl(const Object& obj) const;

    Condition::Ptr weak_condition_;
    CompareType compare_type_;
    double compare_number_;
};

CompareCondition::Ptr CompareConditionPtr();
CompareCondition::Ptr CompareConditionPtr(Condition::Ptr weak_condition,
                                          CompareType type,
                                          double number);
}
}

#endif  // LTR_DECISION_TREE_COMPARE_CONDITION_H_
