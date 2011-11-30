// Copyright 2011 Yandex

#ifndef LTR_UTILITY_NUMERICAL_H_
#define LTR_UTILITY_NUMERICAL_H_

#include <cmath>
#include <limits>

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
const double DoubleEps = 1e-14;

/**
 * Double special value: Not-a-Number.
 */
const double NaN = std::numeric_limits< double >::quiet_NaN();

/**
 * Checks weather x is Not-a-Number.
 */
bool isNaN(double x);

/**
 * Checks for a and b are equal or both a and b are NaNs.
 */
bool equalWithNaN(double a, double b);

/**
 * Functions for comparison of two doubles (proper processing with small diferences)
 */
bool DoubleMore(const double lhs, const double rhs);
bool DoubleLess(const double lhs, const double rhs);
bool DoubleEqual(const double lhs, const double rhs);
bool DoubleMoreOrEqual(const double lhs, const double rhs);
bool DoubleLessOrEqual(const double lhs, const double rhs);
bool DoubleNotEqual(const double lhs, const double rhs);
};
};

#endif  // LTR_UTILITY_NUMERICAL_H_
