// Copyright 2011 Yandex

#include <stdexcept>
#include <utility>
#include <algorithm>

#include "ltr/scorers/composition_scorers/order_statistic_composition_scorer.h"
#include "ltr/utility/numerical.h"

using std::pair;
using std::make_pair;
using std::sort;
using ltr::utility::DoubleEqual;
using ltr::utility::DoubleLess;

double NonNegative(double value) {
  return (value >= 0) ? value : 0;
}

namespace ltr {
  double OrderStaticticCompositionScorer::scoreImpl(const Object& object) const {
    // if weight[i] < 0 then take it 0
    double sum_weight = 0.0;
    vector<pair<double,double> > weighted_scores(size());
    for (size_t i = 0; i < size(); ++i) {
      sum_weight += NonNegative(at(i).weight);
      weighted_scores[i] = make_pair(((*at(i).scorer)(object)), NonNegative(at(i).weight));
    }

    if (DoubleEqual(sum_weight, 0.0)) {
      throw std::logic_error("No positive weights in OrderStaticticCompositionScorer");
    }

    // it might be a faster realization without sort
    double weight_threshold = ratio_ * sum_weight;
    sort(weighted_scores.begin(), weighted_scores.end());

    int index = 0;
    sum_weight = 0.0;
    while (DoubleLess(sum_weight, weight_threshold)) {
      sum_weight += NonNegative(weighted_scores[index].second);
      ++index;
    }

    return weighted_scores[index - 1].first;
  }

  string OrderStaticticCompositionScorer::generateCppCodeImpl
      (const string& function_name) const {
    string hpp_code = "Not implemented";
    return hpp_code;
  }

  string OrderStaticticCompositionScorer::toString() const {
    std::stringstream str;
    std::fixed(str);
    str.precision(2);
    str << "Order statictic composition with ration " << ratio_
      << " of " << size() << " scorers: {\n";
    for (int i = 0; i < size(); ++i) {
      str << at(i).scorer->toString();
      str << " with weight " << at(i).weight << "\n";
    }
    str << "}";
    return str.str();
  }
};
