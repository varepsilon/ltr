// Copyright 2012 Yandex

#include "ltr/optimization/solvers/simulated_annealing_solver.h"
#include "ltr/utility/random_number_generator.h"
#include "ltr/utility/macros.h"

namespace optimization {
State SimulatedAnnealingSolver::selectNextPoint(const Function& function,
  const Set& set, const State& state) {
    Point next_point = set.project(getRandomPoint(state.point()));
    double next_value = function.value(next_point);
    if (needTransition(state.value(), next_value,
      temperature(state.iteration(), max_iterations()))) {
        return State(next_point, state.iteration() + 1, next_value);
      } else {
        return State(state.point(), state.iteration() + 1, state.value());
      }
}

Point SimulatedAnnealingSolver::getRandomPoint(const Point& point) const {
  Point random_point = point;
  for (int i = 0; i < point.size(); ++i) {
    random_point[i] += ltr::utility::randomizer.doubleRand(-jump_area_width(),
      jump_area_width());
  }
  return random_point;
}

bool SimulatedAnnealingSolver::needTransition(
  double prev_value,
  double next_value,
  double temperature) const {
    double probability = 1;
    if (next_value > prev_value)
      probability = exp((prev_value - next_value) / temperature);
    return probability > ltr::utility::randomizer.doubleRand(0, 1);
  }

double SimulatedAnnealingSolver::temperature(int iteration,
  int max_iterations) const {
    CHECK(max_iterations > 0);
    double time = (static_cast<double>(iteration))/(max_iterations + 1);
    return 1 - time;
  }
}
