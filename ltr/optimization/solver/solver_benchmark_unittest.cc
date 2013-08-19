// Copyright 2013 Yandex

#include <gtest/gtest.h>

#include <algorithm>

#include "ltr/optimization/population_initers/naive_initer.h"
#include "ltr/optimization/population_updaters/per_point_updater.hpp"
#include "ltr/optimization/population_updaters/directed_updaters.hpp"
#include "ltr/optimization/stop_criteria/per_point_stop_criterion.hpp"
#include "ltr/optimization/stop_criteria/one_point_stop_criteria.hpp"
#include "ltr/optimization/solver/solver.h"
#include "ltr/optimization/solver/artificial_bee_colony.h"

#include "ltr/optimization/test_problems/unconstrained_functions.h"
#include "ltr/optimization/sets/ball_set.h"
#include "ltr/optimization/sets/cube_set.h"

#include "ltr/optimization/solver/solver_benchmark.h"

namespace optimization {
TEST(SolverBenchmarkTest, FirstSolverBenchmarkTest) {
  SolverBenchmark<DifferentiableFunction> benchmark(10);

  Solver<DifferentiableFunction> solver1(new NaiveIniter,
    new PerPointUpdater<DifferentiableFunction> (
      new SteepestGradientUpdater<DifferentiableFunction>(1, 0.01)),
    new PerPointStopCriterion<DifferentiableFunction> (
      new IterationCountStopCriterion<DifferentiableFunction>));
  solver1.set_alias("First solver");
  benchmark.addSolver(solver1);

  Solver<DifferentiableFunction> solver2(new NaiveIniter,
    new PerPointUpdater<DifferentiableFunction> (
      new SteepestGradientUpdater<DifferentiableFunction>(1, 0.01)),
    new PerPointStopCriterion<DifferentiableFunction> (
      new IterationCountStopCriterion<DifferentiableFunction>));
  solver2.set_alias("Second solver");
  benchmark.addSolver(solver2);

  Point center(2);
  center << 0, 0;

  benchmark.addProblem(new SumSquaresFunction(2), new BallSet(2.0, center));
  benchmark.addProblem(new SumSquaresFunction(2), new BallSet(2.0, center));


  SolverBenchmark<DifferentiableFunction>::Table table = benchmark.run();

  string expected_result = "\n"
"Table (row: Solvers, column: Problems)\n"
"              #1       #2      \n"
"First solver  0        0       \n"
"Second solver 0        0       \n";

  EXPECT_EQ(table.toString(), expected_result);
}

TEST(SolverBenchmarkTest, SolverBenchmarkMultiRunTest) {
  SolverBenchmark<DifferentiableFunction> solver_benchmark(50);

  ArtificialBeeColony<DifferentiableFunction> abc_solver(250);
  abc_solver.set_alias("abc");

  Set::Ptr set(new CubeSet(15.0, 10));

  solver_benchmark.addProblem(new GriewankFunction(10), set);
  solver_benchmark.addSolver(abc_solver);

  EXPECT_ANY_THROW(SolverBenchmark<DifferentiableFunction>::Table table1 =
      solver_benchmark.multiRun(1));

  EXPECT_NO_THROW(SolverBenchmark<DifferentiableFunction>::Table table2 =
      solver_benchmark.multiRun(10));
}
}
