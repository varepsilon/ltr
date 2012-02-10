// Copyright 2011 Yandex

#include <cmath>
#include <algorithm>
#include <stdexcept>

#include "ltr/data/object_list.h"
#include "ltr/measures/dcg.h"
#include "ltr/measures/utils/measure_utility.h"

using std::pow;
using std::log;
using std::sort;
using std::logic_error;
using ltr::utility::PredictedAndActualLabels;
using ltr::utility::ExtractLabels;
using ltr::utility::PredictedDecreasingActualIncreasing;
using ltr::ObjectList;


namespace ltr {
  double DCG::DCGFormula(double relevance, size_t position) {
    return (pow(2, relevance) - 1) / log(position + 2.0);
  }

  double DCG::get_measure(const ObjectList& objects) const {
    vector<PredictedAndActualLabels> labels = ExtractLabels(objects);
    sort(labels.begin(), labels.end(), PredictedDecreasingActualIncreasing);

    size_t n = this->parameters().getInt("NUMBER_OF_OBJECTS_TO_CONSIDER");
    if ((n == 0) || (n > labels.size())) {
      n = labels.size();
    }

    double result = 0.0;
    for (int labels_index = 0; labels_index < n; ++labels_index) {
      result += DCGFormula(labels[labels_index].actual, labels_index);
    }

    if (result < 0.0) {
      throw logic_error(alias() + " calculated < 0");
    }
    return result;
  }
};
