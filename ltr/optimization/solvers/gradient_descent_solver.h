// Copyright 2012 Yandex

#ifndef LTR_OPTIMIZATION_SOLVERS_GRADIENT_DESCENT_SOLVER_H_
#define LTR_OPTIMIZATION_SOLVERS_GRADIENT_DESCENT_SOLVER_H_

#include "ltr/optimization/solvers/iteration_solver.h"
#include "ltr/optimization/functions/differentiable_function.h"
#include "ltr/utility/macros.h"


namespace optimization {
/** \class\brief GradientDescentSolver
 * Implements gradient descent algorithm with constant
 * speed of descent.
 */
class GradientDescentSolver : public IterationSolver<DifferentiableFunction> {
 public:
  /** speed of descent
   */
  GET_SET(double, speed);
 private:
  double speed_;

  virtual State selectNextPoint(const DifferentiableFunction& function,
                                const Set& set,
                                const State& state);
};
}

#endif  // LTR_OPTIMIZATION_SOLVERS_GRADIENT_DESCENT_SOLVER_H_
