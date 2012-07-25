// Copyright 2011 Yandex

#include <logog/logog.h>

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
    INFO("Getting scorer at %d index", index);
    return weighted_scorers_.at(index);
  }
  CompositionScorer::ScorerAndWeight&
      CompositionScorer::at(size_t index) {
    INFO("Getting scorer at %d index", index);
    return weighted_scorers_.at(index);
  }
  const CompositionScorer::ScorerAndWeight&
      CompositionScorer::operator[](size_t index) const {
    INFO("Setting scorer at %d index", index);
    return at(index);
  }
  CompositionScorer::ScorerAndWeight&
      CompositionScorer::operator[](size_t index) {
    INFO("Setting scorer at %d index", index);
    return at(index);
  }

  void CompositionScorer::add
      (const ScorerAndWeight& weighted_scorer) {
    INFO("Adding a scorer and weight");
    weighted_scorers_.push_back(weighted_scorer);
  }

  void CompositionScorer::add
      (Scorer::Ptr scorer, double weight) {
    INFO("Adding a scorer with a weight equal to %lf", weight);
    weighted_scorers_.push_back(ScorerAndWeight(scorer, weight));
  }
};
