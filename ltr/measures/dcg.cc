// Copyright 2011 Yandex

#include <cmath>
#include <algorithm>
#include <vector>
#include <iostream>

#include "ltr/measures/dcg.h"
#include "ltr/utility/numerical.h"

using std::pow;
using std::log;
using std::min;
using std::sort;
using std::vector;

using ltr::utility::DoubleLess;
using ltr::utility::DoubleMore;
using ltr::utility::equalWithNaN;

namespace ltr {

bool sortPredicatedDecreasingActualIncreasingFunc(
    const PredictedAndActualLabels& firstObjectsLabels,
    const PredictedAndActualLabels& secondObjectsLabels) {
  if (DoubleMore(firstObjectsLabels.first, secondObjectsLabels.first)) {
    return true;
  }
  if (equalWithNaN(firstObjectsLabels.first, secondObjectsLabels.first)) {
    return DoubleLess(firstObjectsLabels.second, secondObjectsLabels.second);
  }
  return false;
}

double DCG::get_measure(const ObjectList& objects) const {
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
      sortPredicatedDecreasingActualIncreasingFunc);

  double dcgMeasueValue = 0.0;
  for (int position = 0;
      position < consideredTopObjectsNumber;
      ++position) {
    dcgMeasueValue += iterartion_dcg_formula(
        predictedAndActualLabels[position].second,
        position);
  }

  return dcgMeasueValue;
}

double DCG::iterartion_dcg_formula(double relevance, size_t position) {
  return (pow(2, relevance) - 1) / log(position + 2.0);
}
}
