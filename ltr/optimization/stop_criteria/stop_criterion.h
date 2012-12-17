// Copyright 2012 Yandex

#ifndef LTR_OPTIMIZATION_STOP_CRITERIA_STOP_CRITERION_H_
#define LTR_OPTIMIZATION_STOP_CRITERIA_STOP_CRITERION_H_
#include "ltr/optimization/stop_criteria/state.h"

namespace optimization {
class StopCriterion {
 public:
  virtual bool isTrue(const State& current_state,
                      const State& previous_state = State()) const = 0;
  virtual ~StopCriterion() { }
};
}

#endif  // LTR_OPTIMIZATION_STOP_CRITERIA_STOP_CRITERION_H_
