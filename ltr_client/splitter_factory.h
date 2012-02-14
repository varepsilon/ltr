// Copyright 2012 Yandex

#ifndef LTR_CLIENT_SPLITTER_FACTORY_H_
#define LTR_CLIENT_SPLITTER_FACTORY_H_

#include "ltr/crossvalidation/splitter.h"
#include "ltr_client/factory.h"

typedef Factory<ltr::cv::Splitter> SplitterFactory;

#define REGISTER_LISTWISE_SPLITTER(type, class_name) \
  registerClass<ltr::ObjectList>(type, SimpleFactory<class_name>::create);

#define REGISTER_PAIRWISE_SPLITTER(type, class_name) \
  registerClass<ltr::ObjectPair>(type, SimpleFactory<class_name>::create);

#define REGISTER_POINTWISE_SPLITTER(type, class_name) \
  registerClass<ltr::Object>(type, SimpleFactory<class_name>::create);

#endif  // LTR_CLIENT_SPLITTER_FACTORY_H_

