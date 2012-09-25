// Copyright 2012 Yandex

#ifndef LTR_PREDICTIONS_AGGREGATORS_MAX_WEIGHT_PREDICTIONS_AGGREGATOR_H_
#define LTR_PREDICTIONS_AGGREGATORS_MAX_WEIGHT_PREDICTIONS_AGGREGATOR_H_

#include <vector>
#include <string>

#include "ltr/predictions_aggregators/predictions_aggregator.h"
#include "ltr/parameters_container/parameters_container.h"

using std::vector;

using ltr::ParametersContainer;

namespace ltr {
/**
* Performs max weight aggregation of objects weights and labels
*/
class MaxWeightPredictionsAggregator : public PredictionsAggregator {
 public:
  MaxWeightPredictionsAggregator() {
  }

  explicit MaxWeightPredictionsAggregator(
    const ParametersContainer& parameters) {
  }

  double aggregate(const vector<double>& labels,
                   const vector<double>& weights) {
    if (labels.size() == 0) {
      ERR("There are no labels");
      throw std::logic_error
        ("No labels for MaxWeightPredictionsAggregators");
    } else {
      int best_index = max_element(weights.begin(), weights.end())
        - weights.begin();
      return labels[best_index];
    }
  }

  string generateCppCode(const string& function_name) const {
    string result = "";
    result += "double " + function_name + " (const vector<double>& labels,\n";
    result += "                        const vector<double>& weights) {\n";
    result +=
      " int best_index = max_element(weights.begin(), weights.end())\n";
    result +=
      "  - weights.begin();\n";
    result += " return labels[best_index];\n";
    result += "}\n";
    return result;
  }
 private:
  virtual string getDefaultAlias() const {
    return "Max weight predictions aggregator";
  }
};
};

#endif  // LTR_PREDICTIONS_AGGREGATORS_MAX_WEIGHT_PREDICTIONS_AGGREGATOR_H_

