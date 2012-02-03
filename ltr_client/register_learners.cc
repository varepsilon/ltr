// Copyright 2012 Yandex

#include "ltr_client/learners_initer.h"
#include "ltr/learners/best_feature_learner.h"
#include "ltr/learners/gp_learner.h"


template<class TElement>
typename ltr::BaseLearner<TElement>::Ptr BFIniter
    (const LearnerInfo<TElement>& info) {
  return typename ltr::BaseLearner<TElement>::Ptr(
                          new ltr::BestFeatureLearner<TElement>(info.measure));
}

template <typename TElement>
typename ltr::BaseLearner<TElement>::Ptr GPIniter(
    const LearnerInfo<TElement>& info) {
  return typename ltr::BaseLearner<TElement>::Ptr(
      new ltr::gp::GPLearner<TElement>(info.measure, info.parameters));
}

void LearnerIniter::registerLearners() {
  REGISTER_POINTWISE_LEARNER("BEST_FEATURE", BFIniter<ltr::Object>);
  REGISTER_PAIRWISE_LEARNER("BEST_FEATURE", BFIniter<ltr::ObjectPair>);
  REGISTER_LISTWISE_LEARNER("BEST_FEATURE", BFIniter<ltr::ObjectList>);

  REGISTER_POINTWISE_LEARNER("GP", GPIniter<ltr::Object>);
  REGISTER_PAIRWISE_LEARNER("GP", GPIniter<ltr::ObjectPair>);
  REGISTER_LISTWISE_LEARNER("GP", GPIniter<ltr::ObjectList>);
}
