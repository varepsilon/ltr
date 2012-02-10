// Copyright 2011 Yandex

#include <cmath>
#include <algorithm>
#include <stdexcept>

#include "ltr/data/object_list.h"
#include "ltr/measures/ndcg.h"
#include "ltr/measures/utils/measure_utility.h"
#include "ltr/utility/numerical.h"

using std::pow;
using std::log;
using std::sort;
using std::logic_error;
using ltr::utility::PredictedAndActualLabels;
using ltr::utility::ExtractLabels;
using ltr::utility::PredictedDecreasingActualIncreasing;
using ltr::utility::ActualDecreasing;
using ltr::utility::DoubleEqual;
using ltr::ObjectList;

namespace ltr {
  double NDCG::NDCGFormula(double relevance, size_t position) {
    return (pow(2, relevance) - 1) / log(position + 2.0);
  }

  double NDCG::get_measure(const ObjectList& objects) const {
    vector<PredictedAndActualLabels> labels = ExtractLabels(objects);
    sort(labels.begin(), labels.end(), ActualDecreasing);

    size_t n = this->parameters().getInt("NUMBER_OF_OBJECTS_TO_CONSIDER");
    if ((n == 0) || (n > labels.size())) {
      n = labels.size();
    }

    double best_possible_DCG = 0.0;
    for (int labels_index = 0; labels_index < n; ++labels_index) {
      best_possible_DCG +=
        NDCGFormula(labels[labels_index].actual, labels_index);
    }

    sort(labels.begin(), labels.end(), PredictedDecreasingActualIncreasing);

    double DCG_value = 0.0;
    for (int labels_index = 0; labels_index < n; ++labels_index) {
      DCG_value += NDCGFormula(labels[labels_index].actual, labels_index);
    }

    if (!utility::DoubleEqual(best_possible_DCG, 0.0)) {
      double result =  DCG_value / best_possible_DCG;
      if (result > 1.0) {
        throw logic_error(alias() + " calculated > 1");
      } else if (result < 0.0) {
        throw logic_error(alias() + " calculated < 0");
      }
      return result;
    } else {
      return 0.0;
    }
  }
};
