// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_DECISION_TREE_LEARNER_H_
#define LTR_LEARNERS_DECISION_TREE_LEARNER_H_

#include "ltr/learners/learner.h"

#include "ltr/learners/utility/conditions_learner.h"
#include "ltr/learners/utility/splitting_quality.h"
#include "ltr/scorers/decision_tree_scorer.h"

namespace ltr {

using decision_tree::ConditionsLearner;
using decision_tree::SplittingQuality;

class DecisionTreeLearner
  : public Learner<Object, DecisionTreeScorer> {
  private:
    DecisionTreeScorer scorer_;
    ConditionsLearner::Ptr conditions_learner_;
    SplittingQuality::Ptr splitting_quality_;

    void learnImpl(const DataSet<Object>& data);
    Vertex<double>::Ptr createOneVertex(const DataSet<Object>& data);

  public:
    DecisionTreeScorer make() const {
      return scorer_;
    }
    void setInitialScorer(const DecisionTreeScorer& scorer) {
      scorer_ = scorer;
    }
    DecisionTreeLearner(
        const ParametersContainer& parameters = ParametersContainer());
    void reset() {
      scorer_ = DecisionTreeScorer();
    }
};
}
#endif  // LTR_LEARNERS_DECISION_TREE_LEARNER_H_
