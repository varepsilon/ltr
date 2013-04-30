// Copyright 2013 Yandex

#include "ltr/optimization/stop_criteria/vote_aggregator.h"

namespace optimization {
void VoteAggregator::reset() {
  true_answers_ = 0;
  false_answers_ = 0;
}
void VoteAggregator::push(bool answer) {
  if (answer) {
    true_answers_++;
  } else {
    false_answers_++;
  }
}
bool VoteAggregator::isTrue(double true_answers_threshold) {
  CHECK(true_answers_threshold <= 1.0 &&
    true_answers_threshold >= 0.0);
  return ((double)true_answers() /
           (double)(true_answers() + false_answers())) >=
             true_answers_threshold;
}
}
