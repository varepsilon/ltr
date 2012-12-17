// Copyright 2012 Yandex

#include "ltr/optimization/stop_criteria/and_criterion.h"

namespace optimization {
bool And::isTrue(const State& current_state,
                 const State& previous_state) const {
return (first_.isTrue(current_state, previous_state) &&
        second_.isTrue(current_state, previous_state));
}
}
