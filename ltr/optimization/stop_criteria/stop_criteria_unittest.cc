// Copyright 2013 Yandex

#include <gtest/gtest.h>

#include "ltr/optimization/stop_criteria/one_point_stop_criteria.hpp"
#include "ltr/optimization/stop_criteria/one_point_stop_criterion.h"
#include "ltr/optimization/stop_criteria/and_or_stop_criteria.hpp"
#include "ltr/optimization/test_problems/unconstrained_functions.h"
#include "ltr/optimization/sets/ball_set.h"
#include "ltr/optimization/functions/function.h"
#include "ltr/optimization/stop_criteria/vote_aggregator.h"
#include "ltr/optimization/population_initers/population.h"
#include "ltr/optimization/stop_criteria/per_point_stop_criterion.hpp"

using optimization::IterationCountStopCriterion;
using optimization::DeltaFunctionStopCriterion;
using optimization::DeltaArgumentStopCriterion;
using optimization::GradientMagnitudeStopCriterion;
using optimization::And;
using optimization::Or;
using optimization::Population;
using optimization::SumSquaresFunction;
using optimization::Function;
using optimization::DifferentiableFunction;
using optimization::VoteAggregator;
using optimization::BallSet;
using optimization::PerPointStopCriterion;
using optimization::OnePointStopCriterion;
typedef Eigen::VectorXd Point;
typedef Eigen::VectorXd Vector;
typedef Eigen::MatrixXd Matrix;

TEST(OnePointStopCriterionTest, IterationStopCriterionTest) {
  Point point(2);
  IterationCountStopCriterion<Function>
    iteration_stop_criterion(new SumSquaresFunction(2));
  iteration_stop_criterion.init(point);
  iteration_stop_criterion.set_max_iteration(2);
  iteration_stop_criterion.update(point);

  EXPECT_FALSE(iteration_stop_criterion.isTrue());

  iteration_stop_criterion.update(point);
  iteration_stop_criterion.update(point);

  EXPECT_TRUE(iteration_stop_criterion.isTrue());
}

TEST(OnePointStopCriterionTest, DeltaFunctionStopCriterionTest) {
  Point point(2);
  point.setZero();
  DeltaFunctionStopCriterion<Function>
    iteration_stop_criterion(new SumSquaresFunction(2));
  iteration_stop_criterion.init(point);
  iteration_stop_criterion.set_min_delta(0.01);
  iteration_stop_criterion.update(point);
  EXPECT_TRUE(iteration_stop_criterion.isTrue());
  point.setConstant(100.0);
  iteration_stop_criterion.update(point);
  EXPECT_FALSE(iteration_stop_criterion.isTrue());
}

TEST(OnePointStopCriterionTest, DeltaArgumentStopCriterionTest) {
  Point point(2);
  point.setZero();
  DeltaArgumentStopCriterion<Function>
    iteration_stop_criterion(new SumSquaresFunction(2));
  iteration_stop_criterion.init(point);
  iteration_stop_criterion.set_min_delta(0.01);
  iteration_stop_criterion.update(point);
  EXPECT_TRUE(iteration_stop_criterion.isTrue());
  point.setConstant(100.0);
  iteration_stop_criterion.update(point);
  EXPECT_FALSE(iteration_stop_criterion.isTrue());
}

TEST(OnePointStopCriterionTest, GradientStopCriterionTest) {
  Point point(2);
  point.setZero();
  GradientMagnitudeStopCriterion<DifferentiableFunction>
    iteration_stop_criterion(new SumSquaresFunction(2));
  iteration_stop_criterion.init(point);
  iteration_stop_criterion.set_min_gradient(0.01);
  iteration_stop_criterion.update(point);
  EXPECT_TRUE(iteration_stop_criterion.isTrue());
  point.setConstant(100.0);
  iteration_stop_criterion.update(point);
  EXPECT_FALSE(iteration_stop_criterion.isTrue());
}

TEST(OnePointStopCriterionTest, AndStopCriterionTest) {
  Point point(2);
  And<Function> iteration_stop_criterion(
      new SumSquaresFunction(2),
      new DeltaFunctionStopCriterion<Function>(new SumSquaresFunction(3), 0.1),  // NOLINT
      new IterationCountStopCriterion<Function>(new SumSquaresFunction(3), 3));  // NOLINT

  point << 0, 0;
  iteration_stop_criterion.init(point);
  point << 100, 100;
  iteration_stop_criterion.update(point);
  EXPECT_FALSE(iteration_stop_criterion.isTrue());
  iteration_stop_criterion.update(point);
  EXPECT_FALSE(iteration_stop_criterion.isTrue());
  iteration_stop_criterion.update(point);
  iteration_stop_criterion.update(point);
  EXPECT_TRUE(iteration_stop_criterion.isTrue());
}

TEST(OnePointStopCriterionTest, OrStopCriterionTest) {
  Point point(2);
  Or<Function> iteration_stop_criterion(
      new SumSquaresFunction(2),
      new DeltaFunctionStopCriterion<Function>(new SumSquaresFunction(3), 0.1),
      new IterationCountStopCriterion<Function>(new SumSquaresFunction(3), 3));

  point << 0, 0;
  iteration_stop_criterion.init(point);
  point << 100, 100;
  iteration_stop_criterion.update(point);
  EXPECT_FALSE(iteration_stop_criterion.isTrue());
  iteration_stop_criterion.update(point);
  EXPECT_TRUE(iteration_stop_criterion.isTrue());
  iteration_stop_criterion.update(point);
  iteration_stop_criterion.update(point);
  EXPECT_TRUE(iteration_stop_criterion.isTrue());
}
TEST(OrStopCriterionTest, OrAliasTest) {
  Or<Function> iteration_stop_criterion(
      new SumSquaresFunction(2),
      new DeltaFunctionStopCriterion<Function>(new SumSquaresFunction(3), 0.1),
      new IterationCountStopCriterion<Function>(new SumSquaresFunction(3), 3));

  EXPECT_TRUE("DeltaFunctionStopCriterion_||_IterationCountStopCriterion" ==
                iteration_stop_criterion.alias());
}

TEST(VoteAggregatorTest, VoteAggregatorTest) {
  VoteAggregator aggregator;
  aggregator.push(false);
  EXPECT_FALSE(aggregator.isTrue(0.5));
  aggregator.push(true);
  EXPECT_TRUE(aggregator.isTrue(0.5));
  aggregator.push(true);
  EXPECT_TRUE(aggregator.isTrue(0.5));
}

TEST(OnePointStopCriterionTest, CloneTest) {
  Point point(2);
  IterationCountStopCriterion<Function>
    iteration_stop_criterion(new SumSquaresFunction(2));
  iteration_stop_criterion.set_max_iteration(2);
  OnePointStopCriterion<Function>::Ptr cloned_stop_criterion =
    iteration_stop_criterion.clone();

  iteration_stop_criterion.init(point);
  cloned_stop_criterion->init(point);
  iteration_stop_criterion.update(point);
  cloned_stop_criterion->update(point);
  EXPECT_FALSE(iteration_stop_criterion.isTrue());
  EXPECT_FALSE(cloned_stop_criterion->isTrue());
  cloned_stop_criterion->update(point);
  cloned_stop_criterion->update(point);

  EXPECT_TRUE(cloned_stop_criterion->isTrue());
}

TEST(PerPointStopCriterionTest, IterationCountTest) {
  IterationCountStopCriterion<Function>::Ptr
    one_point_stop_criterion =
      new IterationCountStopCriterion<Function>(new SumSquaresFunction(2));
  one_point_stop_criterion->set_max_iteration(3);

  PerPointStopCriterion<Function> iteration_stop_criterion(one_point_stop_criterion); // NOLINT
  iteration_stop_criterion.set_aggregator_threshold(0.5);

  Population population;
  Point a(2);
  a << 1, 1;
  Point b(2);
  b << 2, 2;
  Point c(2);
  c << 3, 3;

  population.addPoint(a);
  population.addPoint(b);
  population.addPoint(c);

  iteration_stop_criterion.init(population);
  EXPECT_FALSE(iteration_stop_criterion.isTrue());
  iteration_stop_criterion.update(population);
  EXPECT_FALSE(iteration_stop_criterion.isTrue());
  iteration_stop_criterion.update(population);
  EXPECT_FALSE(iteration_stop_criterion.isTrue());
  iteration_stop_criterion.update(population);
  EXPECT_TRUE(iteration_stop_criterion.isTrue());
}

TEST(PerPointStopCriterionTest, DeltaArgumentTest) {
  DeltaArgumentStopCriterion<Function>::Ptr
    one_point_stop_criterion =
      new DeltaArgumentStopCriterion<Function>(new SumSquaresFunction(2));
  one_point_stop_criterion->set_min_delta(0.05);

  PerPointStopCriterion<Function> delta_argument_stop_criterion(one_point_stop_criterion); // NOLINT
  delta_argument_stop_criterion.set_aggregator_threshold(0.5);

  Population population;
  Point a(2);
  a << 1, 1;
  Point b(2);
  b << 2, 2;
  Point c(2);
  c << 3, 3;

  population.addPoint(a);
  population.addPoint(b);
  population.addPoint(c);

  delta_argument_stop_criterion.init(population);
  EXPECT_FALSE(delta_argument_stop_criterion.isTrue());
  Point point(2);
  point << 2, 2;
  population.updatePoint(0, point);
  point << 3, 3;
  population.updatePoint(1, point);
  delta_argument_stop_criterion.update(population);
  EXPECT_FALSE(delta_argument_stop_criterion.isTrue());
  point << 4, 4;
  population.updatePoint(2, point);
  delta_argument_stop_criterion.update(population);
  EXPECT_TRUE(delta_argument_stop_criterion.isTrue());
  delta_argument_stop_criterion.set_aggregator_threshold(0.67);
  EXPECT_FALSE(delta_argument_stop_criterion.isTrue());
}

TEST(PerPointStopCriterionTest, DeltaValueTest) {
  DeltaFunctionStopCriterion<Function>::Ptr
    one_point_stop_criterion =
      new DeltaFunctionStopCriterion<Function>(new SumSquaresFunction(2));
  one_point_stop_criterion->set_min_delta(0.05);

  PerPointStopCriterion<Function> delta_value_stop_criterion(one_point_stop_criterion); // NOLINT
  delta_value_stop_criterion.set_aggregator_threshold(0.5);

  Population population;
  Point a(2);
  a << 1, 1;
  Point b(2);
  b << 2, 2;
  Point c(2);
  c << 3, 3;

  population.addPoint(a);
  population.addPoint(b);
  population.addPoint(c);

  delta_value_stop_criterion.init(population);
  EXPECT_FALSE(delta_value_stop_criterion.isTrue());
  Point point(2);
  point << 2, 2;
  population.updatePoint(0, point);
  point << 3, 3;
  population.updatePoint(1, point);
  delta_value_stop_criterion.update(population);
  EXPECT_FALSE(delta_value_stop_criterion.isTrue());
  point << 4, 4;
  population.updatePoint(2, point);
  delta_value_stop_criterion.update(population);
  EXPECT_TRUE(delta_value_stop_criterion.isTrue());
  delta_value_stop_criterion.set_aggregator_threshold(0.67);
  EXPECT_FALSE(delta_value_stop_criterion.isTrue());
}

TEST(PerPointStopCriterionTest, GradientMagnitudeTest) {
  GradientMagnitudeStopCriterion<DifferentiableFunction>::Ptr
    one_point_stop_criterion =
      new GradientMagnitudeStopCriterion<DifferentiableFunction>(new SumSquaresFunction(2));  // NOLINT
  one_point_stop_criterion->set_min_gradient(0.05);

  PerPointStopCriterion<DifferentiableFunction> gradient_magnitude_stop_criterion(one_point_stop_criterion); // NOLINT
  gradient_magnitude_stop_criterion.set_aggregator_threshold(0.5);

  Population population;
  Point a(2);
  a << 1, 1;
  Point b(2);
  b << 1, 1;
  Point c(2);
  c << 1, 1;

  population.addPoint(a);
  population.addPoint(b);
  population.addPoint(c);

  gradient_magnitude_stop_criterion.init(population);
  EXPECT_FALSE(gradient_magnitude_stop_criterion.isTrue());
  Point point(2);
  point << 2, 2;
  population.updatePoint(0, point);
  point << 3, 3;
  population.updatePoint(1, point);
  gradient_magnitude_stop_criterion.update(population);
  EXPECT_FALSE(gradient_magnitude_stop_criterion.isTrue());
  point << 0, 0;
  population.updatePoint(1, point);
  population.updatePoint(2, point);
  gradient_magnitude_stop_criterion.update(population);
  EXPECT_TRUE(gradient_magnitude_stop_criterion.isTrue());
  gradient_magnitude_stop_criterion.set_aggregator_threshold(0.67);
  EXPECT_FALSE(gradient_magnitude_stop_criterion.isTrue());
}

TEST(PerPointStopCriterionTest, UpdateTest) {
  IterationCountStopCriterion<Function>::Ptr one_point_stop_criterion =
    new IterationCountStopCriterion<Function>(new SumSquaresFunction(2));
  one_point_stop_criterion->set_max_iteration(3);

  PerPointStopCriterion<Function> iteration_stop_criterion(one_point_stop_criterion); // NOLINT
  iteration_stop_criterion.set_aggregator_threshold(0.3);

  Population population;
  Point a(2);
  a << 1, 1;
  Point b(2);
  b << 2, 2;
  Point c(2);
  c << 3, 3;

  population.addPoint(a);
  population.addPoint(b);
  population.addPoint(c);

  iteration_stop_criterion.init(population);
  EXPECT_FALSE(iteration_stop_criterion.isTrue());

  population.addPoint(a);
  population.removePoint(0);
  iteration_stop_criterion.update(population);

  population.addPoint(a);
  population.addPoint(a);
  population.removePoint(2);
  iteration_stop_criterion.update(population);
  EXPECT_FALSE(iteration_stop_criterion.isTrue());

  population.removePoint(4);
  iteration_stop_criterion.update(population);

  EXPECT_TRUE(iteration_stop_criterion.isTrue());
  iteration_stop_criterion.set_aggregator_threshold(0.5);
  EXPECT_FALSE(iteration_stop_criterion.isTrue());

  iteration_stop_criterion.update(population);
  EXPECT_TRUE(iteration_stop_criterion.isTrue());
}
