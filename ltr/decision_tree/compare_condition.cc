// Copyright 2012 Yandex

#include "ltr/decision_tree/compare_condition.h"
#include "ltr/utility/numerical.h"

namespace ltr {
namespace decision_tree {

void CompareCondition::setCompareNumber(double number) {
  compare_number_ = number;
}

void CompareCondition::setCompareType(CompareType type) {
  compare_type_ = type;
}

void CompareCondition::setWeakCondition(Condition::Ptr weak_condition) {
  weak_condition_ = weak_condition;
}

double CompareCondition::value(const ltr::Object &obj) const {
  if (weak_condition_ == NULL)
    throw std::logic_error("no weak condition");
  double weak_value = weak_condition_->value(obj);
  switch (compare_type_) {
    case EQUAL: return utility::DoubleEqual(weak_value, compare_number_);
    case GREATER: return utility::DoubleMore(weak_value, compare_number_);
    case LESS: return utility::DoubleLess(weak_value, compare_number_);
    case GREATER_OR_EQUAL:
            return utility::DoubleMoreOrEqual(weak_value, compare_number_);
    case LESS_OR_EQUAL:
            return utility::DoubleLessOrEqual(weak_value, compare_number_);
    default:
      throw std::logic_error("unknown compare type");
  }
}

CompareCondition::Ptr CompareConditionPtr() {
  return CompareCondition::Ptr(new CompareCondition);
}

CompareCondition::Ptr CompareConditionPtr(Condition::Ptr weak_condition,
                                          CompareType type,
                                          double number) {
  return CompareCondition::Ptr(new CompareCondition(weak_condition,
                                                    type, number));
}
}
}
