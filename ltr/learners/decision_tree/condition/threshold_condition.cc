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

string ThresholdCondition::generateCppCode(const string& function_name) const {
  string hpp_code;

  hpp_code.
    append("inline double ").
    append(function_name + "_id_" + lexical_cast<string>(this->getId())).
    append("(const std::vector<double>& features) {\n  return ").
    append("(features[" + lexical_cast<string>(feature_index_) + "]").
    append(" > " + lexical_cast<string>(left_feature_value_bound_) + ") && ").
    append("(features[" + lexical_cast<string>(feature_index_) + "]").
    append(" <= " + lexical_cast<string>(right_feature_value_bound_) + ");\n").
    append("}\n\n");

  return hpp_code;
}
};
};
