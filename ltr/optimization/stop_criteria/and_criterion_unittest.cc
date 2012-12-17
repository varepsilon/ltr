// Copyright 2011 Yandex

#include <gtest/gtest.h>
#include "ltr/optimization/stop_criteria/state.h"
#include "ltr/optimization/stop_criteria/and_criterion.h"
#include "ltr/optimization/stop_criteria/or_criterion.h"

#include "ltr/optimization/stop_criteria/delta_function_stop_criterion.h"
#include "ltr/optimization/stop_criteria/delta_argument_stop_criterion.h"
#include "ltr/optimization/stop_criteria/iteration_stop_criterion.h"
#include "ltr/optimization/stop_criteria/gradient_stop_criterion.h"

using optimization::DeltaFunctionStopCriterion;
using optimization::DeltaArgumentStopCriterion;
using optimization::IterationStopCriterion;
using optimization::GradientStopCriterion;
using optimization::State;
using optimization::And;
using optimization::Or;

typedef Eigen::VectorXd Vector;
typedef Eigen::VectorXd Point;
typedef Eigen::MatrixXd Matrix;

TEST(CriteriaTest, AndCriteriaTest) {
  Point point1(1);
  Point point2(1);

  point1(0) = 0.5;
  point2(0) = 0.2;

  Vector gradient1(1);
  Vector gradient2(1);

  gradient1(0) = 0.5;
  gradient2(0) = 0.1;

  State state1;
  State state2;

  state1.set_gradient(gradient1);
  state1.set_point(point1);
  state1.set_value(2.0);
  state1.set_iteration(10);

  state2.set_gradient(gradient2);
  state2.set_point(point2);
  state2.set_value(1.0);
  state2.set_iteration(11);

  DeltaFunctionStopCriterion stop1(0.5);  // false
  DeltaArgumentStopCriterion stop2(2.5);  // true
  IterationStopCriterion stop3(7);  // true
  GradientStopCriterion stop4(0.1);  // false

  EXPECT_EQ(true,
            And(stop2, Or(Or(stop1, stop3), stop4)).isTrue(state1, state2));
}
