// Copyright 2011 Yandex

#include <gtest/gtest.h>
#include "ltr/optimization/stop_criteria/state.h"
#include "ltr/optimization/stop_criteria/or_criterion.h"
#include "ltr/optimization/stop_criteria/delta_function_stop_criterion.h"

using optimization::DeltaFunctionStopCriterion;
using optimization::State;
using optimization::Or;

TEST(CriteriaTest, OrCriteriaTest) {
  DeltaFunctionStopCriterion stop1(1.0);
  DeltaFunctionStopCriterion stop2(2.0);

  State state1, state2;

  state1.set_value(2.0);
  state2.set_value(0.5);

  Or or1(stop1, stop2);

  EXPECT_EQ(true, or1.isTrue(state1, state2));
}
