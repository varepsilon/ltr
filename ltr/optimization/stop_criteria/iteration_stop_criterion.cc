// Copyright 2012 Yandex

#include "ltr/optimization/stop_criteria/iteration_stop_criterion.h"
#include "Eigen/Core"

namespace optimization {
bool IterationStopCriterion::isTrue(const State& current_state,
                                    const State& previous_state) const {
  return (current_state.iteration() >= max_iteration_);
}
}
