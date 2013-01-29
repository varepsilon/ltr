// Copyright 2012 Yandex

#ifndef LTR_OPTIMIZATION_SOLVERS_SIMULATED_ANNEALING_SOLVER_H_
#define LTR_OPTIMIZATION_SOLVERS_SIMULATED_ANNEALING_SOLVER_H_

#include "ltr/optimization/solvers/iteration_solver.h"
#include "ltr/optimization/functions/function.h"
#include "ltr/utility/macros.h"

namespace optimization {
/** \class\brief SimulatedAnnealingSolver
 * Implements simulated annealing optimization algorithm
 */
class SimulatedAnnealingSolver : public IterationSolver<Function> {
 public:
  /**
   * next point is selected randomly in a cube with diameter twice the
   * jump_area_width and center in current point
   */
  GET_SET(double, jump_area_width);
  /**
   * number of iterations after which 'temperature' reaches 0
   */
  GET_SET(int, max_iterations);
 private:
  virtual State selectNextPoint(const Function& function,
                                const Set& set,
                                const State& state);
  Point getRandomPoint(const Point& point) const;

  bool needTransition(double prev_value, double next_value,
                      double temperature) const;
  double temperature(int iteration, int max_iterations) const;

  double jump_area_width_;
  int max_iterations_;
};
}

#endif  // LTR_OPTIMIZATION_SOLVERS_SIMULATED_ANNEALING_SOLVER_H_
