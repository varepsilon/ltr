// Copyright 2011 Yandex

#ifndef LTR_SCORERS_LINEAR_COMPOSITION_SCORER_H_
#define LTR_SCORERS_LINEAR_COMPOSITION_SCORER_H_

#pragma once

#include <string>
#include <vector>

#include "scorers/scorer.h"
#include "scorers/utility/scorer_utility.h"

namespace ltr {
  class LinearCompositionScorer : public IScorer {
  public:
    typedef boost::shared_ptr< LinearCompositionScorer > Ptr;

    LinearCompositionScorer() : IScorer("LinearCompositionScorer") {
    }

    struct ScorerAndWeight {
      IScorer::Ptr scorer;
      double weight;

      ScorerAndWeight(IScorer::Ptr r, double w) :
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
    void add(IScorer::Ptr scorer, double weight);

    virtual std::string brief() const;
    virtual double operator()(const Object& obj) const;
    virtual std::string generateCppCode
      (const std::string& class_name, int tabbing) const;
    virtual std::string generateJavaCode
      (const std::string& class_name, int tabbing, bool is_static) const;
  private:
    std::vector< ScorerAndWeight > scorers_;
  };
};

#endif  // LTR_SCORERS_LINEAR_COMPOSITION_SCORER_H_
