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
#include "ltr/optimization/sets/polyhedron_set.h"
#include "ltr/optimization/solver/solvers.hpp"
#include "ltr/utility/random_number_generator.h"
using std::abs;


namespace optimization {
class SolverTest: public testing::Test {
 public:
  BallSet::Ptr set;
  SumSquaresFunction::Ptr function;
  int population_size;

  SolverTest() { }

  void SetUp() {
    Point center(2);
    center << 0, 0;
    set = new BallSet(2.0, center);
    function = new SumSquaresFunction(2);
    population_size = 10;
  }

  void TearDown() { }

  ~SolverTest() { }
};

TEST_F(SolverTest, SolverFirstConstructorTest) {
  Solver<DifferentiableFunction> solver(
            new NaiveIniter,
            new PerPointUpdater<DifferentiableFunction> (
                new SteepestGradientUpdater<DifferentiableFunction>(1.0, 0.01)),
            new PerPointStopCriterion<DifferentiableFunction> (
                new IterationCountStopCriterion<DifferentiableFunction>));

  Solution solution = solver.solve(function, set, population_size);

  EXPECT_TRUE(abs(solution.min_value) < 0.1);
  EXPECT_TRUE(abs(solution.argmin[0]) < 0.1);
  EXPECT_TRUE(abs(solution.argmin[1]) < 0.1);
  EXPECT_EQ(solution.num_iter, 1000);
  EXPECT_TRUE(0 <= solution.time && solution.time <= 1000);
}

TEST_F(SolverTest, SolverSecondConstructorTest) {
  Solver<DifferentiableFunction> solver(
            new SteepestGradientUpdater<DifferentiableFunction>(1.0, 0.01),
            new IterationCountStopCriterion<DifferentiableFunction>);

  Solution solution = solver.solve(function, set, population_size);

  EXPECT_TRUE(abs(solution.min_value) < 0.1);
  EXPECT_TRUE(abs(solution.argmin[0]) < 0.1);
  EXPECT_TRUE(abs(solution.argmin[1]) < 0.1);
  EXPECT_EQ(solution.num_iter, 1000);
  EXPECT_TRUE(0 <= solution.time && solution.time <= 1000);
}

TEST_F(SolverTest, GradientDescentSolverFirstConstructorTest) {
  double fixed_step = 0.1;
  double min_gradient_magnitude = 0.01;
  GradientDescentSolver<DifferentiableFunction> solver(fixed_step,
                                                       min_gradient_magnitude);
  Solution solution;

  solution = solver.solve(function, set, population_size);
  EXPECT_TRUE(abs(solution.argmin[1]) < 0.1);
}

TEST_F(SolverTest, GradientDescentSolverSecondConstructorTest) {
  double fixed_step = 0.1;
  double min_gradient_magnitude = 0.01;
  GradientDescentSolver<DifferentiableFunction> solver(
      fixed_step,
      new PerPointStopCriterion<DifferentiableFunction>(
          new GradientMagnitudeStopCriterion<DifferentiableFunction>(
              min_gradient_magnitude)));
  Solution solution;

  solution = solver.solve(function, set, population_size);
  EXPECT_TRUE(abs(solution.argmin[1]) < 0.1);
}

TEST_F(SolverTest, GradientDescentSolverThirdConstructorTest) {
  double fixed_step = 0.1;
  double min_gradient_magnitude = 0.01;
  GradientDescentSolver<DifferentiableFunction> solver(
      fixed_step,
      new GradientMagnitudeStopCriterion<DifferentiableFunction>(
          min_gradient_magnitude));
  Solution solution;

  solution = solver.solve(function, set, population_size);
  EXPECT_TRUE(abs(solution.argmin[1]) < 0.1);
}

TEST_F(SolverTest, SteepestGradientSolverFirstConstructorTest) {
  double max_step = 0.1;
  double brute_force_step = 0.01;
  double min_gradient_magnitude = 0.01;
  SteepestGradientSolver<DifferentiableFunction> solver(max_step,
                                                        brute_force_step,
                                                        min_gradient_magnitude);
  Solution solution;

  solution = solver.solve(function, set, population_size);
  EXPECT_TRUE(abs(solution.argmin[1]) < 0.1);
}

TEST_F(SolverTest, SteepestGradientSolverSecondConstructorTest) {
  double max_step = 0.1;
  double brute_force_step = 0.01;
  double min_gradient_magnitude = 0.01;
  SteepestGradientSolver<DifferentiableFunction> solver(
      max_step,
      brute_force_step,
      new PerPointStopCriterion<DifferentiableFunction>(
          new GradientMagnitudeStopCriterion<DifferentiableFunction>(
              min_gradient_magnitude)));
  Solution solution;

  solution = solver.solve(function, set, population_size);
  EXPECT_TRUE(abs(solution.argmin[1]) < 0.1);
}

TEST_F(SolverTest, SteepestGradientSolverThirdConstructorTest) {
  double max_step = 0.1;
  double brute_force_step = 0.01;
  double min_gradient_magnitude = 0.01;
  SteepestGradientSolver<DifferentiableFunction> solver(
      max_step,
      brute_force_step,
      new GradientMagnitudeStopCriterion<DifferentiableFunction>(
          min_gradient_magnitude));
  Solution solution;

  solution = solver.solve(function, set, population_size);
  EXPECT_TRUE(abs(solution.argmin[1]) < 0.1);
}

TEST_F(SolverTest, ConjugateGradientSolverFirstConstructorTest) {
  double max_step = 0.1;
  double brute_force_step = 0.01;
  double min_gradient_magnitude = 0.01;
  ConjugateGradientSolver<DifferentiableFunction> solver(max_step,
                                                        brute_force_step,
                                                        min_gradient_magnitude);
  Solution solution;

  solution = solver.solve(function, set, population_size);
  EXPECT_TRUE(abs(solution.argmin[1]) < 0.1);
}

TEST_F(SolverTest, ConjugateGradientSolverSecondConstructorTest) {
  double max_step = 0.1;
  double brute_force_step = 0.01;
  double min_gradient_magnitude = 0.01;
  ConjugateGradientSolver<DifferentiableFunction> solver(
      max_step,
      brute_force_step,
      new PerPointStopCriterion<DifferentiableFunction>(
          new GradientMagnitudeStopCriterion<DifferentiableFunction>(
              min_gradient_magnitude)));
  Solution solution;

  solution = solver.solve(function, set, population_size);
  EXPECT_TRUE(abs(solution.argmin[1]) < 0.1);
}

TEST_F(SolverTest, ConjugateGradientSolverThirdConstructorTest) {
  double max_step = 0.1;
  double brute_force_step = 0.01;
  double min_gradient_magnitude = 0.01;
  ConjugateGradientSolver<DifferentiableFunction> solver(
      max_step,
      brute_force_step,
      new GradientMagnitudeStopCriterion<DifferentiableFunction>(
          min_gradient_magnitude));
  Solution solution;

  solution = solver.solve(function, set, population_size);
  EXPECT_TRUE(abs(solution.argmin[1]) < 0.1);
}
}
