// Copyright 2012 Yandex

#ifndef LTR_OPTIMIZATION_SOLVERS_ITERATION_SOLVER_H_
#define LTR_OPTIMIZATION_SOLVERS_ITERATION_SOLVER_H_

#include "ltr/optimization/solvers/solver.h"

namespace optimization {
template<class FunctionT> class IterationSolver : public Solver<FunctionT> {
 public:
  virtual ~IterationSolver() { }

  Point initial_point() const { return initial_point_; }
  void set_initial_point(const Point& point) { initial_point_ = point; }

  Solution solve(const FunctionT& function, const Set& set) {
    Point current_point = setInitialPoint(set);
    double current_value = function.value(current_point);
    Point best_point = current_point;
    double best_value = current_value;
    int iteration = 0;

    while (!stop(function, set, current_point, iteration)) {
      current_point = selectNextPoint(function, set, current_point,
        iteration);
      current_value = function.value(current_point);
      if (current_value < best_value) {
        best_point = current_point;
        best_value = current_value;
      }
      ++iteration;
    }

    Solution result;
    result.solution = best_point;
    result.value = best_value;
    return result;
  }
 protected:
  virtual Point selectNextPoint(const FunctionT& function,
                                const Set& set,
                                const Point& current_point,
                                int iteration) = 0;
  virtual bool stop(const FunctionT& function,
                    const Set& set,
                    const Point& current_point,
                    int iteration) = 0;
 private:
  Point initial_point_;

  Point setInitialPoint(const Set& set) const {
    return set.project(initial_point());
  }
};
}

#endif  // LTR_OPTIMIZATION_SOLVERS_ITERATION_SOLVER_H_
