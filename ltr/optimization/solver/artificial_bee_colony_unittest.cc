// Copyright 2013 Yandex

#include <gtest/gtest.h>

#include <algorithm>

#include "ltr/optimization/solver/artificial_bee_colony.h"
#include "ltr/optimization/population_updaters/bee_colony_updater.h"
#include "ltr/optimization/test_problems/unconstrained_functions.h"
#include "ltr/optimization/sets/ball_set.h"
#include "ltr/optimization/solver/solution.h"

using std::abs;

namespace optimization {
TEST(ArtificialBeeColonyTest, GriewankFunctionTest) {
  ArtificialBeeColony<DifferentiableFunction> solver;
  DifferentiableFunction::Ptr function(new GriewankFunction(2));
  Point center(2);
  center << 0, 0;
  BallSet::Ptr set(new BallSet(600.0, center));
  int population_size = 10;
  Solution solution = solver.solve(function, set, population_size);
  EXPECT_TRUE(abs(solution.min_value) < 0.1);
  EXPECT_TRUE(abs(solution.argmin[0]) < 0.1);
  EXPECT_TRUE(abs(solution.argmin[1]) < 0.1);
}
}
