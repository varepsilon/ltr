// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_DECISION_TREE_CONDITION_THRESHOLD_CONDITION_H_
#define LTR_LEARNERS_DECISION_TREE_CONDITION_THRESHOLD_CONDITION_H_

#include <string>

#include "ltr/data/object.h"

#include "ltr/learners/decision_tree/condition/condition.h"

namespace ltr {
namespace decision_tree {
/*
 * ThresholdCondition tests object feature for belonging to
 * half-interval (left_feature_value_bound_; right_feature_value_bound_].
 */
class ThresholdCondition : public Condition {
 public:
  typedef ltr::utility::shared_ptr<ThresholdCondition> Ptr;

  ThresholdCondition(int feature_index,
                     double left_feature_value_bound,
                     double right_feature_value_bound)
  : feature_index_(feature_index),
    left_feature_value_bound_(left_feature_value_bound),
    right_feature_value_bound_(right_feature_value_bound) {}
  /*
   * Return 1 if object feature belongs to half-interval and 0 otherwise.
   */
  double value(const Object& object) const;

  virtual string generateCppCode(const string& function_name) const;

 private:
  int feature_index_;
  double left_feature_value_bound_;
  double right_feature_value_bound_;
};
};
};

#endif  // LTR_LEARNERS_DECISION_TREE_CONDITION_THRESHOLD_CONDITION_H_
