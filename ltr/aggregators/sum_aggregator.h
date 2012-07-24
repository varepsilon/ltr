// Copyright 2012 Yandex

#ifndef LTR_AGGREGATORS_SUM_AGGREGATOR_H_
#define LTR_AGGREGATORS_SUM_AGGREGATOR_H_

#include <vector>
#include <string>

#include "ltr/aggregators/aggregator.h"

using std::vector;

namespace ltr {
/**
* Performs sum aggregation of objects weights and labels
*/
class SumAggregator : public Aggregator {
 public:
  double aggregate(const vector<double>& labels,
                   const vector<double>& weights) {
    double result = 0;
    for (int label_index = 0; label_index < labels.size(); ++label_index) {
      result += labels[label_index] * weights[label_index];
    }
    return result;
  }

  string generateCppCode(const string& function_name) const {
    string result = "";
    result += "double " + function_name + " (const vector<double>& labels,\n";
    result += "                        const vector<double>& weights) {\n";
    result += " double result = 0;\n";
    result += " for (int index = 0; index < labels.size(); ++index) {\n";
    result += "   result += labels[index] * weights[index];\n";
    result += " }\n";
    result += " return result;\n";
    result += "}\n";
    return result;
  }
};
};

#endif  // LTR_AGGREGATORS_SUM_AGGREGATOR_H_