// Copyright 2012 Yandex

#ifndef LTR_OPTIMIZATION_SOLVERS_SOLVER_H_
#define LTR_OPTIMIZATION_SOLVERS_SOLVER_H_

#include "ltr/optimization/solvers/solution.h"
#include "ltr/optimization/sets/set.h"

namespace optimization {
/** \class\brief Solver
 * base class for optimization methods
 */
template<class FunctionType> class Solver {
 public:
  virtual ~Solver() { }

  /** finds approximate global minimum of given function on a given set
   */
  virtual Solution solve(const FunctionType& function, const Set& set) = 0;
};
}

#endif  // LTR_OPTIMIZATION_SOLVERS_SOLVER_H_

