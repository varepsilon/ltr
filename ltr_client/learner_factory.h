// Copyright 2012 Yandex

#ifndef LTR_CLIENT_LEARNER_FACTORY_H_
#define LTR_CLIENT_LEARNER_FACTORY_H_

#include "ltr/learners/learner.h"
#include "ltr_client/factory.h"

typedef Factory<ltr::BaseLearner> LearnerFactory;

#define REGISTER_LISTWISE_LEARNER(type, class_name) \
  registerClass<ltr::ObjectList>(type, SimpleFactory<class_name>::create);

#define REGISTER_PAIRWISE_LEARNER(type, class_name) \
  registerClass<ltr::ObjectPair>(type, SimpleFactory<class_name>::create);

#define REGISTER_POINTWISE_LEARNER(type, class_name) \
  registerClass<ltr::Object>(type, SimpleFactory<class_name>::create);

#endif  // LTR_CLIENT_LEARNER_FACTORY_H_
