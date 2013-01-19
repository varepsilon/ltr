// Copyright 2012 Yandex

#include "ltr/optimization/solvers/gradient_descent_solver.h"

namespace optimization {
State GradientDescentSolver::selectNextPoint(
  const DifferentiableFunction& function,
  const Set& set,
  const State& state) {
    Point next_point = set.project(state.point() + state.gradient() * speed());
    return State(next_point, state.iteration() + 1, function.value(next_point),
                 function.gradient(next_point));
}
}
