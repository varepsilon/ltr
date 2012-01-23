// copyright 2011 Yandex

#include "ltr/decision_tree/decision_tree.h"

namespace ltr {
namespace DecisionTree {

bool _cache_conditions = true;
bool _deep_cache_conditions = false;

double Condition::operator()(const Object& obj) const {
  return value(obj);
}

double Condition::value(const Object& obj) const {
  if (!_cache_conditions) {
    return valueImpl(obj);
  } else {
    if (obj == last_object_)
      return last_value_;
    last_value_ = valueImpl(obj);
    if (_deep_cache_conditions)
      last_object_ = obj.deepCopy();
    else
      last_object_ = obj;
    return last_value_;
  }
}
}
}
