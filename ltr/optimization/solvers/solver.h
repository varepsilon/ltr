// Copyright 2012 Yandex

#ifndef LTR_OPTIMIZATION_SOLVERS_SOLVER_H_
#define LTR_OPTIMIZATION_SOLVERS_SOLVER_H_

#include "ltr/optimization/solvers/solution.h"
#include "ltr/optimization/sets/set.h"

namespace optimization {
template<class FunctionType> class Solver {
 public:
  virtual ~Solver() { }

  virtual Solution solve(const FunctionType& function, const Set& set) = 0;
};
}

#endif  // LTR_OPTIMIZATION_SOLVERS_SOLVER_H_

