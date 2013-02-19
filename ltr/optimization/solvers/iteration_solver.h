// Copyright 2012 Yandex

#ifndef LTR_OPTIMIZATION_SOLVERS_ITERATION_SOLVER_H_
#define LTR_OPTIMIZATION_SOLVERS_ITERATION_SOLVER_H_

#include "ltr/optimization/solvers/solver.h"
#include "ltr/optimization/stop_criteria/stop_criterion.h"
#include "ltr/optimization/stop_criteria/state.h"

namespace optimization {
/** \class\brief IterationSolver
 * base class for all iteration methods
 * \tparam TFunction kind of function used in method (differentiable,
 * twice differentiable, etc.)
 */
template<class TFunction> class IterationSolver : public Solver<TFunction> {
 public:
  virtual ~IterationSolver() { }

  GET_SET(Point, initial_point);
  GET_SET(StopCriterion::Ptr, stop_criterion);

  /**
   * finds an approximate solution using iterative method
   */
  Solution solve(const TFunction& function, const Set& set) {
    State previous_state;
    State best_state;
    current_state_.set_point(setInitialPoint(set));
    current_state_.set_value(function.value(current_state_.point()));
    current_state_.set_iteration(0);
    best_state = current_state_;

    do {
      State previous_state = current_state_;
      current_state_ = selectNextPoint(function, set, previous_state);
      if (current_state_.value() < best_state.value()) {
        best_state = current_state_;
      }
    } while (!stop(current_state_, previous_state));

    Solution solution;
    solution.solution = best_state.point();
    solution.value = best_state.value();

    return solution;
  }

  GET(State, current_state);
 protected:
  /** iteration process, different in different algorithms
   * \param state current iteration process state
   */
  virtual State selectNextPoint(const TFunction& function,
                                const Set& set,
                                const State& state) = 0;
 private:
  Point setInitialPoint(const Set& set) const {
    return set.project(initial_point());
  }

  bool stop(const State& curr_state, const State& prev_state) const {
    if (stop_criterion() == StopCriterion::Ptr()) {
      return true;
    }
    return stop_criterion()->isTrue(curr_state, prev_state);
  }

  Point initial_point_;
  StopCriterion::Ptr stop_criterion_;
  State current_state_;
};
}

#endif  // LTR_OPTIMIZATION_SOLVERS_ITERATION_SOLVER_H_
