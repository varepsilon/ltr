// Copyright 2012 Yandex

#ifndef LTR_OPTIMIZATION_STOP_CRITERIA_ITERATION_STOP_CRITERION_H_
#define LTR_OPTIMIZATION_STOP_CRITERIA_ITERATION_STOP_CRITERION_H_

#include "ltr/optimization/stop_criteria/stop_criterion.h"

namespace optimization {
class IterationStopCriterion : public StopCriterion {
 public:
  explicit IterationStopCriterion(int max_iteration)
    : max_iteration_(max_iteration) { }
  ~IterationStopCriterion() { }

  bool isTrue(const State& current_state,
              const State& previous_state = State()) const;

  int max_iteration();
 private:
  int max_iteration_;
};
}
#endif  // LTR_OPTIMIZATION_STOP_CRITERIA_ITERATION_STOP_CRITERION_H_
