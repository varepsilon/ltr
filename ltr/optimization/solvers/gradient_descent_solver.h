// Copyright 2012 Yandex

#ifndef LTR_OPTIMIZATION_SOLVERS_GRADIENT_DESCENT_SOLVER_H_
#define LTR_OPTIMIZATION_SOLVERS_GRADIENT_DESCENT_SOLVER_H_

#include "ltr/optimization/solvers/iteration_solver.h"
#include "ltr/optimization/functions/differentiable_function.h"
#include "ltr/utility/macros.h"

namespace optimization {
class GradientDescentSolver : public IterationSolver<DifferentiableFunction> {
 public:
  GET_SET(double, speed);
  GET_SET(int, max_iterations);
  GET_SET(double, gradient_error);
 private:
  double speed_;
  int max_iterations_;
  double gradient_error_;

  virtual Point selectNextPoint(const DifferentiableFunction& function,
                                const Set& set,
                                const Point& current_point,
                                int iteration);
  virtual bool stop(const DifferentiableFunction& function,
                    const Set& set,
                    const Point& current_point,
                    int iteration);
};
}

#endif  // LTR_OPTIMIZATION_SOLVERS_GRADIENT_DESCENT_SOLVER_H_
