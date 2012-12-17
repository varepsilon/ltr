// Copyright 2012 Yandex

#include "ltr/optimization/stop_criteria/delta_function_stop_criterion.h"
#include "Eigen/Core"

namespace optimization {

bool DeltaFunctionStopCriterion::isTrue(const State& current_state,
                                        const State& previous_state) const {
  double current_value = current_state.value();
  double previous_value = previous_state.value();
  return (abs(current_value - previous_value) <= delta_);
}
}
