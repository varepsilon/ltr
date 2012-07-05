// Copyright 2011 Yandex

#include <boost/lexical_cast.hpp>
#include <string>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include <vector>

#include "ltr/measures/average_precision.h"
#include "ltr/measures/utils/measure_utility.h"

using std::sort;
using std::string;
using std::logic_error;
using ltr::utility::PredictedAndActualLabels;
using ltr::utility::ExtractLabels;
using ltr::utility::PredictedDecreasingActualIncreasing;

namespace ltr {
  double AveragePrecision::get_measure(const ObjectList& objects) const {
    vector<PredictedAndActualLabels> labels = ExtractLabels(objects);
    sort(labels.begin(), labels.end(), PredictedDecreasingActualIncreasing);

    double ans = 0;
    int relevant_amount = 0, total_amount = 1;
    for (vector<PredictedAndActualLabels>::const_iterator labels_it
        = labels.begin(); labels_it != labels.end();
        ++labels_it, total_amount += 1) {
      if (labels_it->actual >=
          score_for_relevant_) {
        relevant_amount += 1;
        ans += static_cast<double>(relevant_amount)/total_amount;
      }
    }
    double metric;
    if (relevant_amount == 0) {
      // todo: log here!
      metric = 0;  // the worst
    } else {
      metric = ans / relevant_amount;
    }
    return metric;
  }

  string AveragePrecision::toString() const {
    std::stringstream str;
    std::fixed(str);
    str.precision(2);
    str << "Average precision measure with parameter SCORE_FOR_RELEVANT = ";
    str << score_for_relevant_;
    return str.str();
  }
};
