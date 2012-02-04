// Copyright 2012 Yandex

#ifndef LTR_CLIENT_LEARNER_FACTORY_H_
#define LTR_CLIENT_LEARNER_FACTORY_H_

#define LEARNER_FACTORY

#include "ltr/learners/learner.h"
#include "ltr_client/factory.h"

#define REGISTER_LISTWISE_LEARNER(type, class_name) \
  registerClass<ltr::ObjectList>(type, SimpleFactory<class_name>::create);

#define REGISTER_PAIRWISE_LEARNER(type, class_name) \
  registerClass<ltr::ObjectPair>(type, SimpleFactory<class_name>::create);

#define REGISTER_POINTWISE_LEARNER(type, class_name) \
  registerClass<ltr::Object>(type, SimpleFactory<class_name>::create);

#undef LEARNER_FACTORY
#endif  // LTR_CLIENT_LEARNER_FACTORY_H_
