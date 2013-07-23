// Copyright 2013 Yandex

#include <gtest/gtest.h>

#include <algorithm>

#include "ltr/optimization/solver/particle_swarm_optimization.h"
#include "ltr/optimization/population_updaters/particle_swarm_updater.h"
#include "ltr/optimization/test_problems/unconstrained_functions.h"
#include "ltr/optimization/sets/ball_set.h"
#include "ltr/optimization/solver/solution.h"

using std::abs;

namespace optimization {
TEST(ParticleSwarmOptimizationTest, GriewankFunctionTest) {
  ParticleSwarmOptimization<DifferentiableFunction> solver;
  DifferentiableFunction::Ptr function(new GriewankFunction(2));
  Point center(2);
  center << 0, 0;
  BallSet::Ptr set(new BallSet(600.0, center));
  int population_size = 100;
  Solution solution = solver.solve(function, set, population_size);
  EXPECT_TRUE(abs(solution.min_value) < 0.1);
}

TEST(ParticleSwarmOptimizationTest, SumSquaresFunctionTest) {
  ParticleSwarmOptimization<DifferentiableFunction> solver;
  Point center(2);
  center << 0, 0;
  BallSet::Ptr set = new BallSet(2.0, center);
  SumSquaresFunction::Ptr function = new SumSquaresFunction(2);
  int population_size = 10;
  Solution solution = solver.solve(function, set, population_size);
  EXPECT_TRUE(abs(solution.min_value) < 0.1);
  EXPECT_TRUE(abs(solution.argmin[0]) < 0.1);
  EXPECT_TRUE(abs(solution.argmin[1]) < 0.1);
}
}
