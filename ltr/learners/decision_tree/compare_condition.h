// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_DECISION_TREE_COMPARE_CONDITION_H_
#define LTR_LEARNERS_DECISION_TREE_COMPARE_CONDITION_H_

#include <string>

#include "ltr/learners/decision_tree/condition.h"

using std::string;

namespace ltr {
namespace decision_tree {

enum CompareType {
  EQUAL,
  GREATER,
  LESS,
  GREATER_OR_EQUAL,
  LESS_OR_EQUAL,
};

/**
Class for condition, which compares value of a weak functor with a constant.
*/
class CompareCondition : public Condition {
  public:
    typedef ltr::utility::shared_ptr<CompareCondition> Ptr;

    /** Sets a compare type.
    */
    void setCompareType(CompareType type);
    /** Sets a constant to compare with
    */
    void setCompareNumber(double number);
    /** Sets weak condition
    */
    void setWeakCondition(Condition::Ptr weak_condition);

    CompareCondition() {}
    CompareCondition(Condition::Ptr weak_condition,
                     CompareType type,
                     double number) :
        weak_condition_(weak_condition),
        compare_type_(type),
        compare_number_(number) {}
    string generateCppCode(const string& function_name) const;

  private:
    double value(const Object& obj) const;

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

#endif  // LTR_LEARNERS_DECISION_TREE_COMPARE_CONDITION_H_
