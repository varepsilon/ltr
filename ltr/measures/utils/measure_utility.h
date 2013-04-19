// Copyright 2011 Yandex

#ifndef LTR_MEASURES_UTILS_MEASURE_UTILITY_H_
#define LTR_MEASURES_UTILS_MEASURE_UTILITY_H_

#include <vector>
#include <utility>

#include "ltr/data/object_list.h"
#include "ltr/data/data_set.h"

using std::vector;
using std::pair;

namespace ltr {
namespace utility {
  /**
   * Type for predicted and actual labels of an object.
   */
  struct PredictedAndActualLabels {
    double predicted;
    double actual;
  };

  template <class TElement>
  vector<PredictedAndActualLabels> ExtractLabels(const TElement& data_set) {
    vector<PredictedAndActualLabels> output(data_set.size());

    for (int index = 0; index < (int)data_set.size(); ++index) {
      output[index].predicted = data_set[index].predicted_label();
      output[index].actual = data_set[index].actual_label();
    }
    return output;
  }

  // first "decreasing" or "increasing" is applied first
  bool PredictedDecreasingActualIncreasing(
    const PredictedAndActualLabels& left,
    const PredictedAndActualLabels& right);

  bool ActualDecreasing(
    const PredictedAndActualLabels& left,
    const PredictedAndActualLabels& right);
};
};

#endif  // LTR_MEASURES_UTILS_MEASURE_UTILITY_H_
