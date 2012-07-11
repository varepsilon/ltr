// Copyright 2011 Yandex

#include "ltr/utility/numerical.h"

using std::abs;

namespace ltr {
namespace utility {

bool isNaN(double x) {
  return x != x;
}

bool DoubleEqualWithNaN(double a, double b) {
  return DoubleEqual(a, b) || (isNaN(a) && isNaN(b));
}

bool DoubleMore(double lhs, double rhs) {
  return !DoubleEqual(lhs, rhs) && lhs > rhs;
}

bool DoubleLess(double lhs, double rhs) {
  return !DoubleEqual(lhs, rhs) && lhs < rhs;
}

bool DoubleEqual(double lhs, double rhs) {
  if (abs(lhs - rhs) < DOUBLE_EPS) {
    return true;
  }	 
  return abs(lhs - rhs) < (abs(lhs) + abs(rhs)) * DOUBLE_TOLERANCE;
}

bool DoubleMoreOrEqual(double lhs, double rhs) {
  return DoubleEqual(lhs, rhs) || DoubleMore(lhs, rhs);  
}

bool DoubleLessOrEqual(double lhs, double rhs) {
  return DoubleEqual(lhs, rhs) || DoubleLess(lhs, rhs);  
}

bool DoubleNotEqual(double lhs, double rhs) {
  return !DoubleEqual(lhs, rhs);
}
};
};
