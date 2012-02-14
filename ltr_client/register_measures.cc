// Copyright 2012 Yandex

#include "ltr_client/measure_factory.h"
#include "ltr/measures/dcg.h"
#include "ltr/measures/ndcg.h"
#include "ltr/measures/abs_error.h"

template <>
void MeasureFactory::registerAll() {
  REGISTER_LISTWISE_MEASURE("DCG", ltr::DCG);
  REGISTER_LISTWISE_MEASURE("NDCG", ltr::NDCG);
  REGISTER_POINTWISE_MEASURE("ABS_ERROR", ltr::AbsError);
}
