// Copyright 2012 Yandex

#include <algorithm>
#include <cmath>
#include <string>
#include <vector>
#include <utility>

#include "ltr/utility/numerical.h"
#include "ltr/measures/utils/measure_utility.h"
#include "ltr/measures/measure.h"
#include "ltr/measures/auc.h"
#include "ltr/data/object.h"
#include "ltr/data/data_set.h"

#include "boost/tuple/tuple.hpp"
#include "boost/tuple/tuple_comparison.hpp"

using std::fabs;
using std::string;
using std::pair;
using std::make_pair;
using std::sort;
using std::vector;

using ltr::Object;
using ltr::DataSet;
using ltr::utility::Inf;
using ltr::utility::PredictedAndActualLabels;
using ltr::utility::PredictedDecreasingActualIncreasing;
using ltr::utility::ExtractLabels;
using ltr::utility::DoubleEqual;


namespace ltr {
  string AUC::toString() const {
    return "AUC Measure";
  }

  double AUC::operator()(const DataSet<Object>& data_set) const {
    vector<tuple<double, double, double> > predicted_actual_weight(
      data_set.size());

    for (int index = 0; index < data_set.size(); ++index) {
      predicted_actual_weight[index] = make_tuple(
        data_set[index].predicted_label(),
        data_set[index].actual_label(),
        data_set.getWeight(index));
    }

    sort(predicted_actual_weight.rbegin(), predicted_actual_weight.rend());

    double result = 0.;
    double true_positive_rate = 0., false_positive_rate = 0.;
    double true_actual = 0., false_actual = 0.;
    for (int index = 0;
         index < predicted_actual_weight.size();
         ++index) {
      double actual_label = predicted_actual_weight[index].get<1>();
      double weight = predicted_actual_weight[index].get<2>();
      if (DoubleEqual(actual_label, 1.)) {
        true_actual += weight;
      } else {
        false_actual += weight;
      }
    }

    for (int index = 0;
         index < predicted_actual_weight.size();
         ++index) {
      double actual_label = predicted_actual_weight[index].get<1>();
      double weight = predicted_actual_weight[index].get<2>();
      if (DoubleEqual(actual_label, 1)) {
        true_positive_rate +=
          weight / true_actual;
      } else {
        result += true_positive_rate *
          weight / false_actual;
        false_positive_rate +=
          weight / false_actual;
      }
    }

    if (DoubleEqual(true_actual, 0)) {
      return 0.;
    }
    if (DoubleEqual(false_actual, 0)) {
      return 1.;
    }

    return result;
  }
};
