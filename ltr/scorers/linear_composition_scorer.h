// Copyright 2011 Yandex

#ifndef LTR_SCORERS_LINEAR_COMPOSITION_SCORER_H_
#define LTR_SCORERS_LINEAR_COMPOSITION_SCORER_H_

#pragma once

#include <string>
#include <vector>

#include "scorers/scorer.h"
#include "scorers/utility/scorer_utility.h"

namespace ltr {
  class LinearCompositionScorer : public Scorer {
  public:
    typedef boost::shared_ptr< LinearCompositionScorer > Ptr;

    LinearCompositionScorer() : Scorer("LinearCompositionScorer") {
    }

    struct ScorerAndWeight {
      Scorer::Ptr scorer;
      double weight;

      ScorerAndWeight(Scorer::Ptr r, double w) :
      scorer(r),
      weight(w) {
      }
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

    virtual std::string brief() const;
    virtual double operator()(const Object& obj) const;

  private:
    virtual string generateCppCodeImpl(const string& function_name) const;
    std::vector< ScorerAndWeight > scorers_;
  };
};

#endif  // LTR_SCORERS_LINEAR_COMPOSITION_SCORER_H_
