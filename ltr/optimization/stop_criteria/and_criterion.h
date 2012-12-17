// Copyright 2012 Yandex

#ifndef LTR_OPTIMIZATION_STOP_CRITERIA_AND_CRITERION_H_
#define LTR_OPTIMIZATION_STOP_CRITERIA_AND_CRITERION_H_
#include "ltr/optimization/stop_criteria/state.h"
#include "ltr/optimization/stop_criteria/stop_criterion.h"
#include "ltr/utility/macros.h"

namespace optimization {
class And : public StopCriterion {
 public:
  explicit And(const StopCriterion& first, const StopCriterion& second) :
  first_(first), second_(second) { }
  bool isTrue(const State& current_state,
              const State& previous_state = State()) const;

 private:
  const StopCriterion& first_;
  const StopCriterion& second_;
};
}
#endif  // LTR_OPTIMIZATION_STOP_CRITERIA_AND_CRITERION_H_
