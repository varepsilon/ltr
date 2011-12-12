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

bool DoubleMore(const double lhs, const double rhs) {
    return lhs - rhs > (abs(lhs)+abs(rhs))*DoubleEps;
}

bool DoubleLess(const double lhs, const double rhs) {
    return lhs - rhs < -(abs(lhs)+abs(rhs))*DoubleEps;
}
bool DoubleEqual(const double lhs, const double rhs) {
    return abs(lhs - rhs) <= (abs(lhs)+abs(rhs))*DoubleEps;
}
bool DoubleMoreOrEqual(const double lhs, const double rhs) {
    return lhs - rhs >= -(abs(lhs)+abs(rhs))*DoubleEps;
}
bool DoubleLessOrEqual(const double lhs, const double rhs) {
    return lhs - rhs <= (abs(lhs)+abs(rhs))*DoubleEps;
}
bool DoubleNotEqual(const double lhs, const double rhs) {
    return abs(lhs - rhs) > (abs(lhs)+abs(rhs))*DoubleEps;
}
};
};
