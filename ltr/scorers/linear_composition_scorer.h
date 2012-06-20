// Copyright 2011 Yandex

#ifndef LTR_SCORERS_LINEAR_COMPOSITION_SCORER_H_
#define LTR_SCORERS_LINEAR_COMPOSITION_SCORER_H_

#pragma once

#include <string>
#include <vector>

#include "ltr/scorers/scorer.h"
#include "ltr/scorers/utility/scorer_utility.h"

using std::string;
using std::vector;

namespace ltr {
class LinearCompositionScorer : public Scorer {
 public:
  typedef boost::shared_ptr<LinearCompositionScorer> Ptr;

  LinearCompositionScorer() {
  }

  struct ScorerAndWeight {
    Scorer::Ptr scorer;
    double weight;

    ScorerAndWeight(Scorer::Ptr r, double w)
      : scorer(r), weight(w) {}
  };

  /**
   * Removes all scorers from combination.
   */
  void clear();

  size_t size() const;

  const ScorerAndWeight& operator[](size_t i) const;
  ScorerAndWeight& operator[](size_t i);
  const ScorerAndWeight& at(size_t i) const;
  ScorerAndWeight& at(size_t i);

  void add(const ScorerAndWeight& scorer);
  void add(Scorer::Ptr scorer, double weight);

  string toString() const;
  virtual double scoreImpl(const Object& obj) const;

 private:
  virtual string generateCppCodeImpl(const string& function_name) const;
  virtual string getDefaultAlias() const {return "LinearCompositionScorer";}
  vector<ScorerAndWeight> scorers_;
};
};

#endif  // LTR_SCORERS_LINEAR_COMPOSITION_SCORER_H_
