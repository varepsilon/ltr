// Copyright 2011 Yandex

#include "ltr/scorers/composition_scorers/median_composition_scorer.h"

namespace ltr {
  string MedianCompositionScorer::toString() const {
    std::stringstream str;
    std::fixed(str);
    str.precision(2);
    str << "Median composition of " << size() << " scorers: {\n";
    for (int i = 0; i < size(); ++i) {
      str << at(i).scorer->toString();
      str << " with weight " << at(i).weight << "\n";
    }
    str << "}";
    return str.str();
  }
};
