// Copyright 2011 Yandex

#ifndef LTR_LEARNERS_COMPOSITION_LEARNER_LINEAR_COMPOSITION_LEARNER_H_
#define LTR_LEARNERS_COMPOSITION_LEARNER_LINEAR_COMPOSITION_LEARNER_H_

#include "ltr/utility/shared_ptr.h"

#include "ltr/learners/composition_learner/composition_learner.h"
#include "ltr/scorers/composition_scorers/linear_composition_scorer.h"


namespace ltr {
namespace composition {
/**
 * A composition learner, what produces a linear composition scorer
 */
template <class TElement>
class LinearCompositionLearner
    : public CompositionLearner<TElement, LinearCompositionScorer> {
  typedef ltr::utility::shared_ptr<LinearCompositionLearner> Ptr;
 private:
  virtual string getDefaultAlias() const {
    return "LinearCompositionLearner";
  }
};
};
};

#endif  // LTR_LEARNERS_COMPOSITION_LEARNER_LINEAR_COMPOSITION_LEARNER_H_
