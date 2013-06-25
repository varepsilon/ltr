// Copyright 2013 Yandex

#include <gtest/gtest.h>

#include "ltr/optimization/population_updaters/per_point_updater.hpp"
#include "ltr/optimization/functions/quadratic_function.h"
#include "ltr/optimization/population_updaters/directed_updater.hpp"
#include "ltr/optimization/population_updaters/directed_updaters.hpp"
#include "ltr/optimization/population_updaters/step_size_calculator.hpp"
#include "ltr/optimization/population_updaters/step_size_calculators.hpp"
#include "ltr/optimization/population_updaters/direction_calculator.hpp"
#include "ltr/optimization/population_updaters/direction_calculators.hpp"
#include "ltr/optimization/test_problems/unconstrained_functions.h"
#include "ltr/optimization/sets/ball_set.h"
#include "ltr/optimization/stop_criteria/one_point_stop_criteria.hpp"
#include "ltr/optimization/stop_criteria/per_point_stop_criterion.hpp"

namespace optimization {
TEST(OnePointUpdatersTest, GradientDescentUpdaterTest) {
  Point center(2);
  center << 0, 0;
  DirectedUpdater<DifferentiableFunction> gradient_updater(
      new GradientDirectionCalculator<DifferentiableFunction>(),
      new ConstantStepCalculator<DifferentiableFunction>(1.0));
  Point init_point;
  gradient_updater.init(init_point, new SumSquaresFunction(2),
      new BallSet(2.0, center));
  Point point(2);
  point << 1, 1;
  Point* updated_point = new Point(2);
  gradient_updater.update(point, &point);
  Point true_point(2);
  true_point << -1, -3;
  EXPECT_TRUE(point.isApprox(true_point, 0.01));

  FixedStepGradientUpdater<DifferentiableFunction> updater(1.0);
  updater.init(init_point, new SumSquaresFunction(2), new BallSet(2.0, center));
  point << 1, 1;
  updater.update(point, &point);
  EXPECT_TRUE(point.isApprox(true_point, 0.01));
}

TEST(OnePointUpdatersTest, SteepestGradientUpdaterTest) {
  Point center(2);
  center << 0, 0;
  DirectedUpdater<DifferentiableFunction> gradient_updater(
      new GradientDirectionCalculator<DifferentiableFunction>(),
      new BruteForceStepCalculator<DifferentiableFunction>(1.0, 0.01));
  Point init_point;
  gradient_updater.init(init_point, new SumSquaresFunction(2),
      new BallSet(2.0, center));
  Point point(2);
  point << 1, 1;
  for (int i = 0; i < 10; ++i) {
    gradient_updater.update(point, &point);
  }
  EXPECT_TRUE(point.isZero(0.01));

  SteepestGradientUpdater<DifferentiableFunction>
    updater(1.0, 0.01);
  updater.init(init_point, new SumSquaresFunction(2), new BallSet(2.0, center));
  point << 1, 1;
  for (int i = 0; i < 10; ++i) {
    updater.update(point, &point);
  }
  EXPECT_TRUE(point.isZero(0.01));
}

TEST(OnePointUpdaterTest, ConjugateGradientUpdater) {
  Point center(2);
  center << 0, 0;
  Point point(2);
  point << 1, 1;
  ConjugateGradientUpdater<DifferentiableFunction>
    updater(1.0, 0.01);
  Point init_point;
  updater.init(init_point, new SumSquaresFunction(2), new BallSet(2.0, center));
  for (int i = 0; i < 10; ++i) {
    updater.update(point, &point);
  }
  EXPECT_TRUE(point.isZero(0.01));
}

TEST(PerPointUpdaterTest, GradientDescentUpdaterTest) {
  Population population;
  Point center(2);
  center << 0, 0;
  DirectedUpdater<DifferentiableFunction>::Ptr gradient_updater =
    new DirectedUpdater<DifferentiableFunction>(
      new GradientDirectionCalculator<DifferentiableFunction>(),
      new BruteForceStepCalculator<DifferentiableFunction>(1.0, 0.01));

  PerPointUpdater<DifferentiableFunction>::Ptr per_point_updater =
    new PerPointUpdater<DifferentiableFunction>(gradient_updater);
  Point a(2);
  a << 1, 1;
  Point b(2);
  b << 2, 2;
  Point c(2);
  c << 3, 3;

  population.addPoint(a);
  population.addPoint(b);
  population.addPoint(c);
  for (int point_id = 1; point_id < 10; ++point_id) {
    population.addPoint(a);
  }
  per_point_updater->init(&population,
                          new SumSquaresFunction(2),
                          new BallSet(2.0, center));
  for (int iteration = 1; iteration < 10; ++iteration) {
    per_point_updater->update(&population);
  }
  population.removePoint(7);
  for (int iteration = 1; iteration < 100; ++iteration) {
    per_point_updater->update(&population);
  }

  for (Population::Iterator it = population.begin();
      it != population.end();
      ++it) {
        EXPECT_TRUE(it.point().isZero(0.01));
  }
}

TEST(PopulationUpdaterTest, SolveTest) {
  Population population;
  Point center(2);
  center << 0, 0;
  SumSquaresFunction::Ptr function = new SumSquaresFunction(2);
  BallSet::Ptr set = new BallSet(2.0, center);

  ConjugateGradientUpdater<DifferentiableFunction>::Ptr one_point_gradient_updater =  // NOLINT
    new ConjugateGradientUpdater<DifferentiableFunction>(1.0, 0.01);  // NOLINT
  GradientMagnitudeStopCriterion<DifferentiableFunction>::Ptr one_point_stop_criterion =  // NOLINT
    new GradientMagnitudeStopCriterion<DifferentiableFunction>;  // NOLINT
  PerPointUpdater<DifferentiableFunction>::Ptr gradient_updater =  // NOLINT
    new PerPointUpdater<DifferentiableFunction>(one_point_gradient_updater);  // NOLINT
  PerPointStopCriterion<DifferentiableFunction>::Ptr stop_criterion =  // NOLINT
    new PerPointStopCriterion<DifferentiableFunction>(one_point_stop_criterion);  // NOLINT
  gradient_updater->init(&population, function, set);
  stop_criterion->init(&population, function, set);
  stop_criterion->set_stop_criteria_quorum(1.0);
  Point a(2);
  a << 1, 1;
  Point b(2);
  b << 2, 2;
  Point c(2);
  c << 3, 3;
  population.addPoint(a);
  population.addPoint(a);
  population.addPoint(a);
  population.addPoint(a);
  population.addPoint(b);
  population.addPoint(c);
  while (!stop_criterion->isTrue()) {
    gradient_updater->update(&population);
    stop_criterion->update(population);
  }
  Point point;
  population.getPoint(0, &point);
  EXPECT_TRUE(point.isZero(0.1));
}
}
