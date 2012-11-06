// Copyright 2012 Yandex

#ifndef LTR_PREDICTIONS_AGGREGATORS_ORDER_STATISTIC_PREDICTIONS_AGGREGATOR_H_
#define LTR_PREDICTIONS_AGGREGATORS_ORDER_STATISTIC_PREDICTIONS_AGGREGATOR_H_

#include <rlog/rlog.h>

#include <vector>
#include <string>
#include <utility>
#include <algorithm>

#include "ltr/predictions_aggregators/predictions_aggregator.h"
#include "ltr/parameters_container/parameters_container.h"
#include "ltr/utility/numerical.h"

using std::pair;
using std::make_pair;
using std::sort;
using std::vector;

using boost::lexical_cast;

using ltr::utility::DoubleEqual;
using ltr::utility::DoubleLess;
using ltr::utility::NonNegative;
using ltr::ParametersContainer;

namespace ltr {
/**
* Performs order statistic aggregation of objects weights and labels
*/
class OrderStatisticPredictionsAggregator : public PredictionsAggregator {
 public:
  OrderStatisticPredictionsAggregator() {
    ratio_ = 0.5;
  }

  explicit OrderStatisticPredictionsAggregator(
    double ratio): ratio_(ratio) {}

  explicit OrderStatisticPredictionsAggregator(
    const ParametersContainer& parameters) {
  }

  double aggregate(const vector<double>& labels,
                   const vector<double>& weights) {
    // if weights[i] < 0 then take it 0
    double sum_weight = 0.0;
    vector<pair<double, double> > weighted_labels(labels.size());
    for (int index = 0; index < labels.size(); ++index) {
      sum_weight += NonNegative(weights[index]);
      weighted_labels[index] = make_pair(labels[index],
        NonNegative(weights[index]));
    }

    if (DoubleEqual(sum_weight, 0.0)) {
      rError("Can't score with no positive-weighted weak labels");
      throw std::logic_error
        ("No positive weights in OrderStatisticPredictionsAggregator");
    }

    // it might be a faster realization without sort
    double weight_threshold = ratio_ * sum_weight;
    sort(weighted_labels.begin(), weighted_labels.end());

    int index = 0;
    sum_weight = 0.0;
    while (DoubleLess(sum_weight, weight_threshold)) {
      sum_weight += NonNegative(weighted_labels[index].second);
      ++index;
    }

    return weighted_labels[index - 1].first;
  }

  string generateCppCode(const string& function_name) const {
    string result = "";
    result +=
      "double " + function_name + " (const vector<double>& labels,\n";
    result +=
      "                        const vector<double>& weights) {\n";
    result +=
      "  double ratio = " + lexical_cast<string>(ratio_) + ";\n";
    result += "  double sum_weight = 0.0;\n";
    result +=
      "  vector<pair<double, double> > weighted_labels(labels.size());\n";
    result += "  for (int index = 0; index < labels.size(); ++index) {\n";
    result += "    sum_weight += NonNegative(weights[index]);\n";
    result += "    weihted_labels[index] =\n";
    result += "      make_pair(labels[index], NonNegative(weights[index]));\n";
    result += "  }\n";
    result += "  if (DoubleEqual(sum_weight, 0.0)) {\n";
    result +=
      "    rError(\"Can't score with no positive-weighted weak labels\");\n";
    result += "    throw std::logic_error\n";
    result += "      (\"No positive weights";
    result += "in OrderStatisticPredictionsAggregator\");\n";
    result += "  }\n";
    result += "  double weight_threshold = ratio * sum_weight;\n";
    result += "  sort(weighted_labels.begin(), weighted_labels.end());\n";
    result += "  int index = 0;\n";
    result += "  sum_weight = 0.0;\n";

    result += "  while (DoubleLess(sum_weight, weight_threshold)) {\n";
    result +=
      "    sum_weight += NonNegative(weighted_labels[index].second);\n";
    result += "    ++index;\n";
    result += "  }\n";

    result += "  return weighted_labels[index - 1].first;\n";
    return result;
  }
 private:
  double ratio_;
  virtual string getDefaultAlias() const {
    return "Order statistic predictions aggregator";
  }
};
};

#endif  // LTR_PREDICTIONS_AGGREGATORS_ORDER_STATISTIC_PREDICTIONS_AGGREGATOR_H_

