// Copyright 2012 Yandex

#include "ltr_client/measures_initer.h"
#include "ltr/measures/dcg.h"
#include "ltr/measures/ndcg.h"

void MeasureIniter::registerMeasures() {
  REGISTER_LISTWISE_MEASURE("DCG", ltr::DCG);
  REGISTER_LISTWISE_MEASURE("NDCG", ltr::NDCG);
}
