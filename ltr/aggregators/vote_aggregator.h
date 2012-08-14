// copyright 2012 Yandex

#ifndef LTR_AGGREGATORS_VOTE_AGGREGATOR_H_
#define LTR_AGGREGATORS_VOTE_AGGREGATOR_H_

#include <map>
#include <string>
#include <vector>

#include "ltr/aggregators/aggregator.h"

using std::vector;
using std::map;

namespace ltr {
/**
* Performs voting aggregation of objects weights and labels
*/
class VoteAggregator : public Aggregator {
 public:
  double aggregate(const vector<double>& labels,
                   const vector<double>& weights) {
    map<double, double> voting;
    for (int label_index = 0; label_index < (int)labels.size(); ++label_index) {
      voting[labels[label_index]] += weights[label_index];
    }
    double result = voting.begin()->first;
    for (map<double, double>::iterator voting_iterator = voting.begin();
         voting_iterator != voting.end();
         ++voting_iterator) {
      if (voting_iterator->second > voting[result]) {
        result = voting_iterator->first;
      }
    }
    return result;
  }

  string generateCppCode(const string& function_name) const {
    string result;
    result += "double " + function_name + " (const vector<double>& labels,\n";
    result += "                           const vector<double>& weights) {\n";
    result += "  map<double, double> voting;\n";
    result += "  for (int index = 0; index < labels.size(); ++index) {\n";
    result += "    voting[labels[index]] += weights[index];\n";
    result += "  }\n";
    result += "  double result = voting.begin()->first;\n";
    result += "  for (map<double, double>::iterator it = voting.begin();\n";
    result += "       it != voting.end();\n";
    result += "       ++it) {\n";
    result += "    if (it->second > voting[result]) {\n";
    result += "      result = it->first;\n";
    result += "    }\n";
    result += "  }\n";
    result += "  return result;\n";
    result += "}\n";
    return result;
  }
 private:
  virtual string getDefaultAlias() const {return "Vote aggregator";}
};
};

#endif  // LTR_AGGREGATORS_VOTE_AGGREGATOR_H_
