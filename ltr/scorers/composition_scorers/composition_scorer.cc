// Copyright 2011 Yandex

#include <logog/logog.h>

#include "ltr/scorers/composition_scorers/composition_scorer.h"

using std::string;

namespace ltr {
namespace composition {
  void CompositionScorer::clear() {
    weighted_scorers_.clear();
  }

  size_t CompositionScorer::size() const {
    return weighted_scorers_.size();
  }

  const CompositionScorer::ScorerAndWeight&
      CompositionScorer::at(size_t scorer_index) const {
    DBUG("Getting scorer at %d index", scorer_index);
    return weighted_scorers_.at(scorer_index);
  }
  CompositionScorer::ScorerAndWeight&
      CompositionScorer::at(size_t scorer_index) {
    DBUG("Getting scorer at %d index", scorer_index);
    return weighted_scorers_.at(scorer_index);
  }
  const CompositionScorer::ScorerAndWeight&
      CompositionScorer::operator[](size_t scorer_index) const {
    DBUG("Setting scorer at %d index", scorer_index);
    return at(scorer_index);
  }
  CompositionScorer::ScorerAndWeight&
      CompositionScorer::operator[](size_t scorer_index) {
    DBUG("Setting scorer at %d index", scorer_index);
    return at(scorer_index);
  }

  void CompositionScorer::add
      (const ScorerAndWeight& weighted_scorer) {
    DBUG("Adding a scorer and weight equal to %lf", weighted_scorer.weight);
    weighted_scorers_.push_back(weighted_scorer);
  }

  void CompositionScorer::add
      (Scorer::Ptr scorer, double weight) {
    add(ScorerAndWeight(scorer, weight));
  }

  double CompositionScorer::scoreImpl(const Object& object) const {
    vector<double> weights(size()), labels(size());
    for (int index = 0; index < size(); ++index) {
      weights[index] = at(index).weight;
      labels[index] = at(index).scorer->score(object);
    }
    return predictions_aggregator_->aggregate(labels, weights);
  }
};
};
