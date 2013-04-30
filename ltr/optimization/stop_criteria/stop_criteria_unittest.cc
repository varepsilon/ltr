// Copyright 2013 Yandex

#include <gtest/gtest.h>

#include "ltr/optimization/stop_criteria/one_point_stop_criteria.hpp"
#include "ltr/optimization/stop_criteria/and_or_stop_criteria.hpp"
#include "ltr/optimization/test_problems/unconstrained_functions.h"
#include "ltr/optimization/sets/ball_set.h"
#include "ltr/optimization/functions/function.h"
#include "ltr/optimization/stop_criteria/vote_aggregator.h"
#include "ltr/optimization/population_initers/population.h"

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
