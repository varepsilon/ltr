// Copyright 2011 Yandex

#ifndef LTR_SCORERS_COMPOSITION_SCORERS_COMPOSITION_SCORER_H_
#define LTR_SCORERS_COMPOSITION_SCORERS_COMPOSITION_SCORER_H_

#include <string>
#include <vector>

#include "ltr/scorers/scorer.h"
#include "ltr/interfaces/parameterized.h"

using std::string;
using std::vector;

namespace ltr {
namespace composition {
/**
 * Scorer, which contains other scorers with their weights. The way of
 * aggregating that weighted scorers is specified in descendant classes
 */
class CompositionScorer : public Scorer, public Parameterized {
 public:
  typedef ltr::utility::shared_ptr<CompositionScorer> Ptr;
  /**
   * Holds a scorer with its weight
   */
  struct ScorerAndWeight {
    Scorer::Ptr scorer;
    double weight;

    ScorerAndWeight(Scorer::Ptr scorer_, double weight_)
      : scorer(scorer_), weight(weight_) {}
  };
  /**
   * Removes all scorers from composition
   */
  virtual void clear();
  /**
   * Returns number of weighted scorers in composition
   */
  size_t size() const;
  /**
   * Returns the weighted scorer by its index in composition
   */
  const ScorerAndWeight& operator[](size_t scorer_index) const;
  /**
   * Returns the weighted scorer by its index in composition
   */
  ScorerAndWeight& operator[](size_t scorer_index);
  /**
   * Returns the weighted scorer by its index in composition
   */
  const ScorerAndWeight& at(size_t scorer_index) const;
  /**
   * Returns the weighted scorer by its index in composition
   */
  ScorerAndWeight& at(size_t scorer_index);
  /**
   * Appends the weighted scorer to the composition
   */
  void add(const ScorerAndWeight& weighted_scorer);
  /**
   * Appends the scorer with its weight to the composition
   */
  void add(Scorer::Ptr scorer, double weight = 1.0);

  virtual ~CompositionScorer() {}

 private:
  virtual string getDefaultAlias() const {return "CompositionScorer";}
  /**
   * An implementation for add function. Is virtual for possibility of
   * optimizing scoring of a composition by calculating some extras in
   * this method. See MaxWeightCompositionScorer as example.
   */
  virtual void addImpl(const ScorerAndWeight& weighted_scorer);
 protected:
  vector<ScorerAndWeight> weighted_scorers_;
};
};
};

#endif  // LTR_SCORERS_COMPOSITION_SCORERS_COMPOSITION_SCORER_H_
