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
#include "ltr/optimization/solver/particle_swarm_optimization.h"

#include "ltr/optimization/test_problems/unconstrained_functions.h"
#include "ltr/optimization/test_problems/constrained_functions.h"
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

  LexicalCastConfig::getInstance().restoreDefaults();
  SolverBenchmark<DifferentiableFunction>::Table table = benchmark.run();

  string expected_result = "\n"
    "Table (row: Solvers, column: Problems)\n"
    "              #1       #2      \n"
    "First solver  0        0       \n"
    "Second solver 0        0       \n";

  EXPECT_EQ(expected_result, table.toString());
}

TEST(SolverBenchmarkTest, SolverBenchmarkMultiRunTest) {
  SolverBenchmark<Function> solver_benchmark(50);

  ArtificialBeeColony<Function> abc_solver(50);
  abc_solver.set_alias("abc");

  ParticleSwarmOptimization<Function> pso_solver(50);
  pso_solver.set_alias("pso");

  solver_benchmark.addSolver(abc_solver);
  solver_benchmark.addSolver(pso_solver);

  solver_benchmark.addProblem(new ConstFunction(2, 5.0), new CubeSet(15.0, 2));
  solver_benchmark.addProblem(new ConstFunction(9, 3.5), new CubeSet(15.0, 9));

  EXPECT_ANY_THROW(SolverBenchmark<DifferentiableFunction>::Table table1 =
      solver_benchmark.multiRun(1));

  EXPECT_NO_THROW(SolverBenchmark<DifferentiableFunction>::Table table2 =
      solver_benchmark.multiRun(10));

  string check_table = "\n"
    "Table (row: Solvers, column: Problems)\n"
    "         #1           #2          \n"
    "abc      5.000+-0.000 3.500+-0.000\n"
    "pso      5.000+-0.000 3.500+-0.000\n";

  LexicalCastConfig::getInstance().restoreDefaults();
  LexicalCastConfig::getInstance().setPrecision(3);
  LexicalCastConfig::getInstance().setFixed(true);
  EXPECT_EQ(check_table, solver_benchmark.multiRun(15).toString());
}
}
