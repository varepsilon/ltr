// Copyright 2012 Yandex

#ifndef LTR_OPTIMIZATION_STOP_CRITERIA_DELTA_ARGUMENT_STOP_CRITERION_H_
#define LTR_OPTIMIZATION_STOP_CRITERIA_DELTA_ARGUMENT_STOP_CRITERION_H_

#include "ltr/optimization/stop_criteria/stop_criterion.h"
#include "ltr/utility/macros.h"

namespace optimization {
class DeltaArgumentStopCriterion : public StopCriterion {
 public:
  explicit DeltaArgumentStopCriterion(double delta) : delta_(delta) { }
  ~DeltaArgumentStopCriterion() { }

  bool isTrue(const State& current_state, const State& previous_state) const;

  GET(double, delta);
 private:
  double delta_;
};
}

#endif  // LTR_OPTIMIZATION_STOP_CRITERIA_DELTA_ARGUMENT_STOP_CRITERION_H_
