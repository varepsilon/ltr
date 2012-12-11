// Copyright 2012 Yandex

#ifndef LTR_OPTIMIZATION_SOLVERS_SIMULATED_ANNEALING_SOLVER_H_
#define LTR_OPTIMIZATION_SOLVERS_SIMULATED_ANNEALING_SOLVER_H_

#include "ltr/optimization/solvers/iteration_solver.h"
#include "ltr/optimization/functions/function.h"
#include "ltr/utility/macros.h"

namespace optimization {
class SimulatedAnnealingSolver : public IterationSolver<Function> {
 public:
  GET_SET(double, jump_area_width);
  GET_SET(int, max_iterations);
 private:
  double jump_area_width_;
  int max_iterations_;

  virtual Point selectNextPoint(const Function& function,
                                const Set& set,
                                const Point& current_point,
                                int iteration);
  virtual bool stop(const Function& function,
                    const Set& set,
                    const Point& current_point,
                    int iteration);

  Point getRandomPoint(const Point& point) const;

  bool needTransition(double prevValue, double nextValue,
    double temperature) const;
  double temperature(int iteration, int maxIterations) const;
};
}

#endif  // LTR_OPTIMIZATION_SOLVERS_SIMULATED_ANNEALING_SOLVER_H_
