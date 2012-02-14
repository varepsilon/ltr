// Copyright 2012 Yandex

#include "ltr_client/learner_factory.h"
#include "ltr/learners/best_feature_learner.h"
#include "ltr/learners/gp_learner.h"
#include "ltr/learners/decision_tree_learner.h"

using ltr::Object;
using ltr::ObjectPair;
using ltr::ObjectList;

using ltr::BestFeatureLearner;
using ltr::gp::GPLearner;
using ltr::DecisionTreeLearner;

void LearnerFactory::registerAll() {
  REGISTER_POINTWISE_LEARNER("BEST_FEATURE", BestFeatureLearner<Object>);
  REGISTER_PAIRWISE_LEARNER("BEST_FEATURE", BestFeatureLearner<ObjectPair>);
  REGISTER_LISTWISE_LEARNER("BEST_FEATURE", BestFeatureLearner<ObjectList>);

  REGISTER_POINTWISE_LEARNER("GP", GPLearner<Object>);
  REGISTER_PAIRWISE_LEARNER("GP", GPLearner<ObjectPair>);
  REGISTER_LISTWISE_LEARNER("GP", GPLearner<ObjectList>);

  REGISTER_POINTWISE_LEARNER("DECISION_TREE", DecisionTreeLearner);
}
