// Copyright 2012 Yandex

#include "ltr_client/learner_factory.h"
#include "ltr/learners/best_feature_learner.h"
#include "ltr/learners/gp_learner.h"
#include "ltr/learners/gp_learner_determinant_strategy.h"

using ltr::Object;
using ltr::ObjectPair;
using ltr::ObjectList;

using ltr::BestFeatureLearner;
using ltr::gp::GPLearner;
using ltr::gp::GPLearnerWithDeterminantStrategy;

template<>
void LearnerFactory::registerAll() {
  REGISTER_POINTWISE_LEARNER("BEST_FEATURE", BestFeatureLearner<Object>);
  REGISTER_PAIRWISE_LEARNER("BEST_FEATURE", BestFeatureLearner<ObjectPair>);
  REGISTER_LISTWISE_LEARNER("BEST_FEATURE", BestFeatureLearner<ObjectList>);

  REGISTER_POINTWISE_LEARNER("GP", GPLearner<Object>);
  REGISTER_PAIRWISE_LEARNER("GP", GPLearner<ObjectPair>);
  REGISTER_LISTWISE_LEARNER("GP", GPLearner<ObjectList>);

  REGISTER_POINTWISE_LEARNER("GP_DETERMINANT_STRATEGY", \
      GPLearnerWithDeterminantStrategy<Object>);
  REGISTER_PAIRWISE_LEARNER("GP_DETERMINANT_STRATEGY", \
      GPLearnerWithDeterminantStrategy<ObjectPair>);
  REGISTER_LISTWISE_LEARNER("GP_DETERMINANT_STRATEGY", \
      GPLearnerWithDeterminantStrategy<ObjectList>);
}
