// Copyright 2013 Yandex

#include <gtest/gtest.h>

#include <algorithm>

#include "ltr/optimization/population_initers/naive_initer.h"
#include "ltr/optimization/population_updaters/per_point_updater.hpp"
#include "ltr/optimization/population_updaters/directed_updaters.hpp"
#include "ltr/optimization/stop_criteria/per_point_stop_criterion.hpp"
#include "ltr/optimization/stop_criteria/one_point_stop_criteria.hpp"
#include "ltr/optimization/solver/solver.h"

#include "ltr/optimization/test_problems/unconstrained_functions.h"
#include "ltr/optimization/sets/ball_set.h"

using std::abs;

namespace optimization {
TEST(SolverTest, FirstSolverTest) {
  Solver<DifferentiableFunction> solver(
            new NaiveIniter,
            new PerPointUpdater<DifferentiableFunction> (
                new SteepestGradientUpdater<DifferentiableFunction>(1.0, 0.01)),
            new PerPointStopCriterion<DifferentiableFunction> (
                new IterationCountStopCriterion<DifferentiableFunction>));

  Point center(2);
  center << 0, 0;
  BallSet::Ptr set = new BallSet(2.0, center);

  SumSquaresFunction::Ptr function = new SumSquaresFunction(2);

  int population_size = 10;

  Solution solution = solver.solve(function, set, population_size);

  EXPECT_TRUE(abs(solution.min_value) < 0.1);
  EXPECT_TRUE(abs(solution.argmin[0]) < 0.1);
  EXPECT_TRUE(abs(solution.argmin[1]) < 0.1);
  EXPECT_TRUE(solution.num_iter == 1000);
  EXPECT_TRUE(0 <= solution.time && solution.time <= 1000);
}
}
