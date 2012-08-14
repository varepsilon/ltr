// Copyright 2011 Yandex

#include <vector>

#include "ltr/measures/utils/measure_utility.h"
#include "ltr/data/object_list.h"
#include "ltr/utility/numerical.h"

using std::vector;

using ltr::utility::DoubleLess;
using ltr::utility::DoubleMore;
using ltr::utility::DoubleEqualWithNaN;

namespace ltr {
namespace utility {

  vector<PredictedAndActualLabels> ExtractLabels(
      const ObjectList& object_list) {
    vector<PredictedAndActualLabels> output(object_list.size());

    for (int index = 0; index < (int)object_list.size(); ++index) {
      output[index].predicted = object_list[index].predicted_label();
      output[index].actual = object_list[index].actual_label();
    }
    return output;
  }

  bool PredictedDecreasingActualIncreasing(
      const PredictedAndActualLabels& left,
      const PredictedAndActualLabels& right) {
    if (DoubleMore(left.predicted, right.predicted)) {
      return true;
    }
    if (DoubleEqualWithNaN(left.predicted, right.predicted)) {
      return DoubleLess(left.actual, right.actual);
    }
    return false;
  }

  bool ActualDecreasing(
      const PredictedAndActualLabels& left,
      const PredictedAndActualLabels& right) {
    return DoubleMore(left.actual, right.actual);
  }
};
};
