// Copyright 2011 Yandex

#include <stdexcept>

#include <logog/logog.h>

#include "ltr/scorers/composition_scorers/max_weight_composition_scorer.h"
#include "ltr/utility/numerical.h"

using ltr::utility::DoubleLess;

namespace ltr {
namespace composition {
  void MaxWeightCompositionScorer::addImpl(const ScorerAndWeight& weighted_scorer) {
    weighted_scorers_.push_back(weighted_scorer);
    if (best_index_ == INVALID_INDEX ||
        DoubleLess(at(best_index_).weight, weighted_scorer.weight)) {
      best_index_ = static_cast<int>(size()) - 1;
    }
  }

  void MaxWeightCompositionScorer::clear() {
    weighted_scorers_.clear();
    best_index_ = INVALID_INDEX;
  }

  double MaxWeightCompositionScorer::scoreImpl(const Object& object) const {
    if (best_index_ == INVALID_INDEX) {
      ERR("Can't score with no weak scorers");
      throw std::logic_error
        ("No weak scorers for MaxWeightCompositionScorer");
    } else {
      return at(best_index_).scorer->score(object);
    }
  }

  string MaxWeightCompositionScorer::generateCppCodeImpl
      (const string& function_name) const {
    string code = at(best_index_).scorer->generateCppCode();
    code.append("inline double ").
      append(function_name).
      append("(const std::vector< double >& features) {\n").
      append("\treturn ").
      append(at(best_index_).scorer->getDefaultSerializableObjectName()).
      append("(features);\n}\n");
    return code;
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
};