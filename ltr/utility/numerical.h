// Copyright 2011 Yandex

#ifndef LTR_UTILITY_NUMERICAL_H_
#define LTR_UTILITY_NUMERICAL_H_

#include <cmath>
#include <limits>

#undef max
#undef min

using std::numeric_limits;

namespace ltr {
namespace utility {
const int DOUBLE_PRECISION = 16;

/**
 * ln(2.0)
 */
const double ln_2 = std::log(2.0);

/**
 * Very small double positive value.
 */
const double DOUBLE_EPS = 1e-14;

const double DOUBLE_TOLERANCE = 1e-14;

/**
 * Very big double positive value.
 */
const double Inf = numeric_limits<double>::max();

/**
 * Double special value: Not-a-Number.
 */
const double NaN = numeric_limits<double>::quiet_NaN();

/**
 * Checks weather x is Not-a-Number.
 */
bool isNaN(double x);

/**
 * Checks for a and b are equal or both a and b are NaNs.
 */
bool DoubleEqualWithNaN(double a, double b);

/**
 * Functions for comparison of two doubles (proper processing with small diferences)
 */
bool DoubleMore(double lhs, double rhs);
bool DoubleLess(double lhs, double rhs);

/**
 * Function for checking whether two doubles equal or not
 * see motivation here: http://www.boost.org/doc/libs/1_34_1/libs/test/doc/components/test_tools/floating_point_comparison.html
 */
bool DoubleEqual(double lhs, double rhs);
bool DoubleMoreOrEqual(double lhs, double rhs);
bool DoubleLessOrEqual(double lhs, double rhs);
bool DoubleNotEqual(double lhs, double rhs);

template <class T>
T NonNegative(T value) {
  return (value >= 0) ? value : 0;
}
};
};

#endif  // LTR_UTILITY_NUMERICAL_H_
