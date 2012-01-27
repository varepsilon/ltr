// Copyright 2011 Yandex

#include <cmath>
#include <algorithm>
#include <iostream>

#include "ltr/measures/ndcg.h"
#include "ltr/utility/numerical.h"

using std::min;
using std::pow;
using std::sort;

namespace ltr {

bool sortActualDecreasingFunc(
    const PredictedAndActualLabels& firstObjectsLabels,
    const PredictedAndActualLabels& secondObjectsLabels) {
  return firstObjectsLabels.second > secondObjectsLabels.second;
}

double NDCG::iterartion_dcg_formula(double relevance, size_t position) {
  return (pow(2, relevance) - 1) / log(position + 2);
}


double NDCG::get_measure(const ObjectList& objects) const {
  int consideredTopObjectsNumber  =
      this->parameters().getInt("NUMBER_OF_OBJECTS_TO_CONSIDER");

  consideredTopObjectsNumber = consideredTopObjectsNumber == 0 ?
      objects.size() : min<int>(consideredTopObjectsNumber, objects.size());

  assert(consideredTopObjectsNumber >= 0);

  if (predictedAndActualLabels.size() != objects.size()) {
    predictedAndActualLabels.resize(objects.size());
  }

  for (size_t objIdx = 0; objIdx < objects.size(); ++objIdx) {
    predictedAndActualLabels[objIdx].first = objects[objIdx].predictedLabel();
    predictedAndActualLabels[objIdx].second = objects[objIdx].actualLabel();
  }

  sort(predictedAndActualLabels.begin(), predictedAndActualLabels.end(),
      sortActualDecreasingFunc);

  double bestPossibleDCG = 0.0;
  for (int position = 0;
      position < consideredTopObjectsNumber;
      ++position) {
    bestPossibleDCG += iterartion_dcg_formula(
        predictedAndActualLabels[position].second,
        position);
  }

  sort(predictedAndActualLabels.begin(), predictedAndActualLabels.end(),
      sortPredicatedDecreasingActualIncreasingFunc);

  double dcgMeasueValue = 0.0;
  for (int position = 0;
      position < consideredTopObjectsNumber;
      ++position) {
    dcgMeasueValue += iterartion_dcg_formula(
        predictedAndActualLabels[position].second,
        position);
  }

  if (!utility::DoubleEqual(bestPossibleDCG, 0)) {
    return dcgMeasueValue / bestPossibleDCG;
  } else {
    return 0.0;
  }
}
}
