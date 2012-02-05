// Copyright 2012 Yandex

#ifndef LTR_CLIENT_MEASURE_FACTORY_H_
#define LTR_CLIENT_MEASURE_FACTORY_H_

#define MEASURE_FACTORY

#include "ltr/measures/measure.h"
#include "ltr_client/factory.h"

#define REGISTER_LISTWISE_MEASURE(type, class_name) \
  registerClass<ltr::ObjectList>(type, SimpleFactory<class_name>::create);

#define REGISTER_PAIRWISE_MEASURE(type, class_name) \
  registerClass<ltr::ObjectPair>(type, SimpleFactory<class_name>::create);

#define REGISTER_POINTWISE_MEASURE(type, class_name) \
  registerClass<ltr::Object>(type, SimpleFactory<class_name>::create);

#undef MEASURE_FACTORY
#endif  // LTR_CLIENT_MEASURE_FACTORY_H_
