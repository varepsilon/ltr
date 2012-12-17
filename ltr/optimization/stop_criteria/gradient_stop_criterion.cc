// Copyright 2012 Yandex

#include "ltr/optimization/stop_criteria/gradient_stop_criterion.h"
#include "Eigen/Core"

namespace optimization {
bool GradientStopCriterion::isTrue(const State& current_state,
                                   const State& previous_state) const {
  Vector current_gradient = current_state.gradient();
  return (current_gradient.norm() <= delta_);
}
}
