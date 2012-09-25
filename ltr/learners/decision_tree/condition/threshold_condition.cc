// Copyright 2012 Yandex

#include "ltr/learners/decision_tree/condition/threshold_condition.h"

#include "ltr/utility/numerical.h"

using ltr::utility::DoubleMore;
using ltr::utility::DoubleLessOrEqual;

namespace ltr {
namespace decision_tree {
double ThresholdCondition::value(const Object& object) const {
  return
    DoubleMore(
      object[feature_index_], left_feature_value_bound_) &&
    DoubleLessOrEqual(
      object[feature_index_], right_feature_value_bound_);
}
};
};
