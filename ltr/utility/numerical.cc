// Copyright 2011 Yandex

#include "ltr/utility/numerical.h"

using std::abs;

namespace ltr {
namespace utility {

bool isNaN(double x) {
  return x != x;
}

bool equalWithNaN(double a, double b) {
  return DoubleEqual(a, b) || (isNaN(a) && isNaN(b));
}

bool DoubleMore(double lhs, double rhs) {
  return lhs - rhs > (abs(lhs) + abs(rhs)) * DoubleEps;
}

bool DoubleLess(double lhs, double rhs) {
  return lhs - rhs < - (abs(lhs) + abs(rhs)) * DoubleEps;
}
bool DoubleEqual(double lhs, double rhs) {
  return abs(lhs - rhs) <= (abs(lhs) + abs(rhs)) * DoubleEps;
}
bool DoubleMoreOrEqual(double lhs, double rhs) {
  return lhs - rhs >= - (abs(lhs) + abs(rhs)) * DoubleEps;
}
bool DoubleLessOrEqual(double lhs, double rhs) {
  return lhs - rhs <= (abs(lhs) + abs(rhs)) * DoubleEps;
}
bool DoubleNotEqual(double lhs, double rhs) {
  return abs(lhs - rhs) > (abs(lhs) + abs(rhs)) * DoubleEps;
}
};
};
