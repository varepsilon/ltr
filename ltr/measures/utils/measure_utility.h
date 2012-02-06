// Copyright 2011 Yandex

#ifndef LTR_MEASURES_UTILS_MEASURE_UTILITY_H_
#define LTR_MEASURES_UTILS_MEASURE_UTILITY_H_

#include <vector>
#include <utility>

#include "ltr/data/object_list.h"

using std::vector;
using std::pair;

namespace ltr {
namespace utility {
  /** \typedef
   * Type for predicted and actual labels of an object.
   */
  struct PredictedAndActualLabels {
    double predicted;
    double actual;
  };

  vector<PredictedAndActualLabels> ExtractLabels(const ObjectList& data_set);

  // first "decreasing" or "increasing" is applied first
  bool PredictedDecreasingActualIncreasing(
    const PredictedAndActualLabels& left,
    const PredictedAndActualLabels& right);

};
};

#endif  // LTR_MEASURES_UTILS_MEASURE_UTILITY_H_
