// Copyright 2012 Yandex

#include "ltr_client/learners_initer.h"

template<class TElement>
typename ltr::BaseLearner<TElement>::Ptr BFIniter
    (const LearnerInfo<TElement>& info) {
  return typename ltr::BaseLearner<TElement>::Ptr(
                          new ltr::BestFeatureLearner<TElement>(info.measure));
}

void LearnerIniter::registerLearners() {
  REGISTER_POINTWISE_LEARNER("BEST_FEATURE", BFIniter<ltr::Object>);
  REGISTER_PAIRWISE_LEARNER("BEST_FEATURE", BFIniter<ltr::ObjectPair>);
  REGISTER_LISTWISE_LEARNER("BEST_FEATURE", BFIniter<ltr::ObjectList>);
}
