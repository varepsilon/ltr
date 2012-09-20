// Copyright 2012 Yandex

#ifndef LTR_PREDICTIONS_AGGREGATORS_AVERAGE_PREDICTIONS_AGGREGATOR_H_
#define LTR_PREDICTIONS_AGGREGATORS_AVERAGE_PREDICTIONS_AGGREGATOR_H_

#include <vector>
#include <string>

#include "ltr/predictions_aggregators/predictions_aggregator.h"
#include "ltr/parameters_container/parameters_container.h"

using std::vector;

using ltr::ParametersContainer;

namespace ltr {
/**
* Performs average aggregation of objects weights and labels
*/
class AveragePredictionsAggregator : public PredictionsAggregator {
 public:

  AveragePredictionsAggregator() {
  }

  explicit AveragePredictionsAggregator(
    const ParametersContainer& parameters) {
  }

  double aggregate(const vector<double>& labels,
                   const vector<double>& weights) {
    double result = 0;
    double weights_sum = 0;
    for (int label_index = 0;
         label_index < (int)labels.size();
         ++label_index) {
      result += labels[label_index] * weights[label_index];
      weights_sum += weights[label_index];
    }
    return result / weights_sum;
  }

  string generateCppCode(const string& function_name) const {
    string result = "";
    result += "double " + function_name + " (const vector<double>& labels,\n";
    result += "                        const vector<double>& weights) {\n";
    result += " double result = 0;\n";
    result += " for (int index = 0; index < labels.size(); ++index) {\n";
    result += "   result += labels[index] * weights[index];\n";
    result += " }\n";
    result += " return result / labels.size();\n";
    result += "}\n";
    return result;
  }
 private:
  virtual string getDefaultAlias() const {
    return "Average predictions aggregator";
  }
};
};

#endif  // LTR_PREDICTIONS_AGGREGATORS_AVERAGE_PREDICTIONS_AGGREGATOR_H_
