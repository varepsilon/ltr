// Copyright 2011 Yandex

#include <boost/lexical_cast.hpp>
#include <string>
#include <algorithm>

#include "ltr/measures/average_precision.h"
#include "ltr/measures/utils/measure_utility.h"

using std::sort;
using std::string;
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
      if (labels_it->actual >= parameters().getDouble("SCORE_FOR_RELEVANT")) {
        relevant_amount += 1;
        ans += static_cast<double>(relevant_amount)/total_amount;
      }
    }
    if (relevant_amount == 0) {
      string str;
      str.append("No relevants for some query on relevant score = ").
        append(boost::lexical_cast<string>(
          parameters().getDouble("SCORE_FOR_RELEVANT")));
      throw std::logic_error(str);
    }
    double metric = ans / relevant_amount;
    if (metric > 1.0) {
      throw std::logic_error("Average Precision calculated > 1");
    }
    return metric;
  }
};
