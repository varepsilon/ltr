// Copyright 2012 Yandex

#include "ltr/optimization/solvers/gradient_descent_solver.h"

namespace optimization {
Point GradientDescentSolver::selectNextPoint(
  const DifferentiableFunction& function,
  const Set& set,
  const Point& current_point,
  int iteration) {
    Vector gradient = function.gradient(current_point);
    return current_point + (gradient * speed());
}

bool GradientDescentSolver::stop(
  const DifferentiableFunction& function,
  const Set& set,
  const Point& current_point,
  int iteration) {
    // TODO(wd28): think about calculating gradient only once
    Vector gradient = function.gradient(current_point);
    return gradient.norm() < gradient_error() ||
      iteration > max_iterations();
}
}
