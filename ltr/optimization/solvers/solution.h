// Copyright 2012 Yandex

#ifndef LTR_OPTIMIZATION_SOLVERS_SOLUTION_H_
#define LTR_OPTIMIZATION_SOLVERS_SOLUTION_H_

#include "ltr/parameters_container/parameters_container.h"
#include "ltr/optimization/functions/function.h"

namespace optimization {
struct Solution {
 public:
  Point solution;
  double value;
};
}

#endif  // LTR_OPTIMIZATION_SOLVERS_SOLUTION_H_
