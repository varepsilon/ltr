// Copyright 2012 Yandex

#include "ltr_client/measure_factory.h"

#include "ltr/measures/abs_error.h"
#include "ltr/measures/accuracy.h"
#include "ltr/measures/squared_error.h"
#include "ltr/measures/true_point.h"

#include "ltr/measures/dcg.h"
#include "ltr/measures/ndcg.h"
#include "ltr/measures/average_precision.h"
#include "ltr/measures/reciprocal_rank.h"
#include "ltr/measures/pfound.h"
#include "ltr/measures/gmrr.h"

template <>
void MeasureFactory::registerAll() {
  REGISTER_POINTWISE_MEASURE("ABS_ERROR", ltr::AbsError);
  REGISTER_POINTWISE_MEASURE("POINTWISE_ACCURACY", ltr::Accuracy<ltr::Object>);
  REGISTER_POINTWISE_MEASURE("SQUARED_ERROR", ltr::SquaredError);
  REGISTER_POINTWISE_MEASURE("TRUE_POINT", ltr::TruePoint);

  REGISTER_PAIRWISE_MEASURE("PAIRWISE_ACCURACY",
    ltr::Accuracy<ltr::ObjectPair>);

  REGISTER_LISTWISE_MEASURE("DCG", ltr::DCG);
  REGISTER_LISTWISE_MEASURE("YANDEX_DCG", ltr::YandexDCG);
  REGISTER_LISTWISE_MEASURE("NDCG", ltr::NDCG);
  REGISTER_LISTWISE_MEASURE("YANDEX_NDCG", ltr::YandexNDCG);
  REGISTER_LISTWISE_MEASURE("AVERAGE_PRECISION", ltr::AveragePrecision);
  REGISTER_LISTWISE_MEASURE("RECIPROCAL_RANK", ltr::ReciprocalRank);
  REGISTER_LISTWISE_MEASURE("PFOUND", ltr::PFound);
  REGISTER_LISTWISE_MEASURE("GMRR", ltr::GMRR);
}
