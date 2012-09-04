// Copyright 2011 Yandex

#include <stdexcept>
#include <utility>
#include <algorithm>

#include <logog/logog.h>

#include "ltr/scorers/composition_scorers/order_statistic_composition_scorer.h"
#include "ltr/utility/numerical.h"

using std::pair;
using std::make_pair;
using std::sort;
using ltr::utility::DoubleEqual;
using ltr::utility::DoubleLess;
using ltr::utility::NonNegative;

namespace ltr {
namespace composition {
  double OrderStatisticCompositionScorer::scoreImpl
      (const Object& object) const {
    // if weight[i] < 0 then take it 0
    double sum_weight = 0.0;
    vector<pair<double, double> > weighted_scores(size());
    for (size_t scorer_index = 0; scorer_index < size(); ++scorer_index) {
      sum_weight += NonNegative(at(scorer_index).weight);
      weighted_scores[scorer_index] = make_pair(at(scorer_index).scorer->score(object),
        NonNegative(at(scorer_index).weight));
    }

    if (DoubleEqual(sum_weight, 0.0)) {
      ERR("Can't score with no positive-weighted weak scorers");
      throw std::logic_error
        ("No positive weights in OrderStatisticCompositionScorer");
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

  string OrderStatisticCompositionScorer::generateCppCodeImpl
      (const string& function_name) const {
    string code = "#include \"ltr/utility/numerical.h\"\n";
    code.append("#include <vector>\n").
      append("#include <utility>\n").
      append("#include <algorithm>\n");
    for (int i = 0; i < size(); ++i) {
      code.append(at(i).scorer->generateCppCode());
    };

    double sum_weight = 0.0;
    code.append("double ").
      append(function_name).
      append("(const std::vector< double >& features) {\n").
      append("\tstd::vector<std::pair<double, double> > weighted_scores("
      + boost::lexical_cast<string>(size()) + ");\n");

    for(int i = 0; i < size(); ++i) {
      sum_weight += NonNegative(at(i).weight);
      code.append("\tweighted_scores[" + boost::lexical_cast<string>(i) + "] = ").
        append("std::make_pair(" + at(i).scorer->getDefaultSerializableObjectName() + "(features), ").
        append(boost::lexical_cast<string>(NonNegative(at(i).weight)) + ");\n");
    }

    double weight_threshold = ratio_ * sum_weight;
    code.append("\n\tstd::sort(weighted_scores.begin(), weighted_scores.end());\n");

    code.append("\tint index = 0;\n").
      append("\tdouble sum_weight = 0.0;\n").
      append("\twhile (ltr::utility::DoubleLess(sum_weight, "
      + boost::lexical_cast<string>(weight_threshold) + ")) {\n").
      append("\t\tsum_weight += ltr::utility::NonNegative(weighted_scores[index].second);\n").
      append("\t\t++index;\n\t}\n");

    code.append("\treturn weighted_scores[index - 1].first;\n").
      append("}\n");

    return code;
  }

  string OrderStatisticCompositionScorer::toString() const {
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
};