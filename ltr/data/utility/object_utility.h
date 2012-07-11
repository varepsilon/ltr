// Copyright 2011 Yandex

#ifndef LTR_DATA_UTILITY_OBJECT_UTILITY_H_
#define LTR_DATA_UTILITY_OBJECT_UTILITY_H_

#include "ltr/utility/numerical.h"

namespace ltr {
  /**
   * Function checks the sameness of two vectors of features
   */
bool DoubleEqualWithNaN(const Features& lhs, const Features& rhs) {
  if (lhs.size() != rhs.size()) {
      return false;
  }
  for (int feature_index = 0;
       feature_index < (int)lhs.size();
       ++feature_index) {
    if (!utility::DoubleEqualWithNaN(lhs[feature_index],
                               rhs[feature_index])) {
      return false;
    }
  }
  return true;
}
  /**
   * Less-comparator by predicted label
   */
class LessPredicted {
 public:
  bool operator()(const Object& lhs, const Object& rhs) const {
    return lhs.predicted_label() < rhs.predicted_label();
  };
};
/**
  * Less-comparator by actual label
  */
class LessActual {
 public:
  bool operator()(const Object& lhs, const Object& rhs) const {
    return lhs.actual_label() < rhs.actual_label();
  };
};
/**
 * Greater-comparator by predicted label
 */
class GreaterPredicted {
 public:
  bool operator()(const Object& lhs, const Object& rhs) const {
    return lhs.predicted_label() > rhs.predicted_label();
  };
};
/**
  * Greater-comparator by actual label
  */
class GreaterActual {
 public:
  bool operator()(const Object& lhs, const Object& rhs) const {
    return lhs.actual_label() > rhs.actual_label();
  };
};
}

#endif  // LTR_DATA_UTILITY_OBJECT_UTILITY_H_

