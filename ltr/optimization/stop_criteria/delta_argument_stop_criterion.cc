// Copyright 2011 Yandex

#include "ltr/optimization/stop_criteria/delta_argument_stop_criterion.h"
#include "Eigen/Core"

namespace optimization {
bool DeltaArgumentStopCriterion::isTrue(const State& current_state,
                                        const State& previous_state) const {
  Point x_current = current_state.point();
  Point x_previous = previous_state.point();
  return ((x_current - x_previous).norm() <= delta_);
}
}
