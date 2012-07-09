// Copyright 2011 Yandex

#ifndef LTR_SCORERS_COMPOSITION_SCORERS_COMPOSITION_SCORER_H_
#define LTR_SCORERS_COMPOSITION_SCORERS_COMPOSITION_SCORER_H_

#include <string>
#include <vector>

#include "ltr/scorers/scorer.h"

using std::string;
using std::vector;

namespace ltr {
class CompositionScorer : public Scorer {
 public:
  typedef boost::shared_ptr<CompositionScorer> Ptr;

  struct ScorerAndWeight {
    Scorer::Ptr scorer;
    double weight;

    ScorerAndWeight(Scorer::Ptr scorer_, double weight_)
      : scorer(scorer_), weight(weight_) {}
  };

  /**
   * Removes all scorers from combination.
   */
  void clear();

  size_t size() const;

  const ScorerAndWeight& operator[](size_t index) const;
  ScorerAndWeight& operator[](size_t index);
  const ScorerAndWeight& at(size_t index) const;
  ScorerAndWeight& at(size_t index);

  void add(const ScorerAndWeight& weighted_scorer);
  void add(Scorer::Ptr scorer, double weight);

  virtual ~CompositionScorer() {}

 private:
  virtual string getDefaultAlias() const {return "CompositionScorer";}
  vector<ScorerAndWeight> weighted_scorers_;
};
};

#endif  // LTR_SCORERS_COMPOSITION_SCORERS_COMPOSITION_SCORER_H_
