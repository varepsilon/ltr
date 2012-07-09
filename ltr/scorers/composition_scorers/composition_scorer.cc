// Copyright 2011 Yandex

#include "ltr/scorers/composition_scorers/composition_scorer.h"

using std::string;

namespace ltr {
  void CompositionScorer::clear() {
    weighted_scorers_.clear();
  }

  size_t CompositionScorer::size() const {
    return weighted_scorers_.size();
  }

  const CompositionScorer::ScorerAndWeight&
      CompositionScorer::at(size_t index) const {
    return weighted_scorers_.at(index);
  }
  CompositionScorer::ScorerAndWeight&
      CompositionScorer::at(size_t index) {
    return weighted_scorers_.at(index);
  }
  const CompositionScorer::ScorerAndWeight&
      CompositionScorer::operator[](size_t index) const {
    return at(index);
  }
  CompositionScorer::ScorerAndWeight&
      CompositionScorer::operator[](size_t index) {
    return at(index);
  }

  void CompositionScorer::add
      (const ScorerAndWeight& weighted_scorer) {
    weighted_scorers_.push_back(weighted_scorer);
  }

  void CompositionScorer::add
      (Scorer::Ptr scorer, double weight) {
    weighted_scorers_.push_back(ScorerAndWeight(scorer, weight));
  }
};
