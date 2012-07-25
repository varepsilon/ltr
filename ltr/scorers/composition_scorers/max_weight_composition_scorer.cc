// Copyright 2011 Yandex

#include <logog/logog.h>

#include "ltr/scorers/composition_scorers/max_weight_composition_scorer.h"
#include "ltr/utility/numerical.h"

using ltr::utility::DoubleLess;

namespace ltr {
  double MaxWeightCompositionScorer::scoreImpl(const Object& object) const {
    INFO("Starting to score an object.");
    size_t current_best_index = 0;
    double current_biggest_weight = at(current_best_index).weight;
    for (size_t index = 1; index < size(); ++index) {
      if (DoubleLess(current_biggest_weight, at(index).weight)) {
        current_best_index = index;
        current_biggest_weight = at(current_best_index).weight;
      }
    }
    return (*at(current_best_index).scorer)(object);
  }

  string MaxWeightCompositionScorer::generateCppCodeImpl
      (const string& function_name) const {
    string hpp_code = "Not implemented";
    return hpp_code;
  }

  string MaxWeightCompositionScorer::toString() const {
    std::stringstream str;
    std::fixed(str);
    str.precision(2);
    str << "Max weight composition of " << size() << " scorers: {\n";
    for (int i = 0; i < size(); ++i) {
      str << at(i).scorer->toString();
      str << " with weight " << at(i).weight << "\n";
    }
    str << "}";
    return str.str();
  }
};
