// Copyright 2012 Yandex

#ifndef LTR_OPTIMIZATION_STOP_CRITERIA_GRADIENT_STOP_CRITERION_H_
#define LTR_OPTIMIZATION_STOP_CRITERIA_GRADIENT_STOP_CRITERION_H_

#include "ltr/optimization/stop_criteria/stop_criterion.h"
#include "ltr/utility/macros.h"

namespace optimization {

class GradientStopCriterion : public StopCriterion {
 public:
  explicit GradientStopCriterion(double delta) : delta_(delta) { }
  ~GradientStopCriterion() { }

  bool isTrue(const State& current_state,
              const State& previous_state = State()) const;

  GET(double, delta)

 private:
  double delta_;
};
}

#endif  // LTR_OPTIMIZATION_STOP_CRITERIA_GRADIENT_STOP_CRITERION_H_
