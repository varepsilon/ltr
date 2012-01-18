// copyright 2011 Yandex

#include "ltr/decision_tree/decision_tree.h"

namespace ltr {
namespace DecisionTree {

double Condition::operator()(const Object& obj) const {
  return value(obj);
}

double Condition::value(const Object& obj) const {
  if (obj == last_object_)
    return last_value_;
  return last_value_ = valueImpl(last_object_ = obj);
}
}
}
