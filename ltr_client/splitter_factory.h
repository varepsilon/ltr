// Copyright 2012 Yandex

#ifndef LTR_CLIENT_SPLITTER_FACTORY_H_
#define LTR_CLIENT_SPLITTER_FACTORY_H_

#define SPLITTER_FACTORY

#include "ltr/crossvalidation/splitter.h"
#include "ltr_client/factory.h"

#define REGISTER_LISTWISE_SPLITTER(type, class_name) \
  registerClass<ltr::ObjectList>(type, SimpleFactory<class_name>::create);

#define REGISTER_PAIRWISE_SPLITTER(type, class_name) \
  registerClass<ltr::ObjectPair>(type, SimpleFactory<class_name>::create);

#define REGISTER_POINTWISE_SPLITTER(type, class_name) \
  registerClass<ltr::Object>(type, SimpleFactory<class_name>::create);


#undef SPLITTER_FACTORY
#endif  // LTR_CLIENT_SPLITTER_FACTORY_H_

