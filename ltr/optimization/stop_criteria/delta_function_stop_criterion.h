// Copyright 2012 Yandex

#ifndef LTR_OPTIMIZATION_STOP_CRITERIA_DELTA_FUNCTION_STOP_CRITERION_H_
#define LTR_OPTIMIZATION_STOP_CRITERIA_DELTA_FUNCTION_STOP_CRITERION_H_

#include "ltr/optimization/stop_criteria/stop_criterion.h"
#include "ltr/utility/macros.h"

namespace optimization {
class DeltaFunctionStopCriterion : public StopCriterion {
 public:
  explicit DeltaFunctionStopCriterion(double delta) : delta_(delta) { }
  ~DeltaFunctionStopCriterion() { }

  bool isTrue(const State& current_state,
              const State& previous_state = State()) const;

  GET(double, delta)
 private:
  double delta_;
};
}

#endif  // LTR_OPTIMIZATION_STOP_CRITERIA_DELTA_FUNCTION_STOP_CRITERION_H_
