// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_DECISION_TREE_DECISION_TREE_LEARNER_H_
#define LTR_LEARNERS_DECISION_TREE_DECISION_TREE_LEARNER_H_

#include <vector>

#include "ltr/learners/learner.h"

#include "ltr/scorers/decision_tree_scorer.h"

#include "ltr/learners/decision_tree/utility/utility.h"
#include "ltr/learners/decision_tree/decision_vertex.h"
#include "ltr/learners/decision_tree/leaf_vertex.h"

#include "ltr/learners/decision_tree/splitting_quality.h"
#include "ltr/learners/decision_tree/conditions_learner.h"

#include "utility/logger.h"
using logger::LogStream;

namespace ltr {
namespace decision_tree {

/**
 \class DecisionTreeLearner
 Builds decision tree for given data.
 */

class DecisionTreeLearner
  : public Learner<Object, DecisionTreeScorer> {
  private:
    DecisionTreeScorer scorer_;
    /** Stream used for logging during learning.
     */
    LogStream log;
    /** Object, used to generate different conditions for splitting data set
     */
    ConditionsLearner::Ptr conditions_learner_;
    /** Object, used to select the best split of the data set
     */
    SplittingQuality::Ptr splitting_quality_;

    /**
      Function creates one decision or leaf vertex for given data.
      Uses ConditionsLearner and SplittingQuality to create it.
     */
    Vertex<double>::Ptr createOneVertex(const DataSet<Object>& data);

    void learnImpl(const DataSet<Object>& data) {
      log << "Learn started. Data set size: " << data.size() << std::endl;
      scorer_.setTree(DecisionTree<double>());
      scorer_.setTreeRoot(createOneVertex(data));
    }
  public:
    string toString() const;

    /** Sets conditiona learner object.
     */
    void setConditionsLearner(ConditionsLearner::Ptr conditions_learner) {
      conditions_learner_ = conditions_learner;
      removeParametersGroup("conditions learner");
      addParametersGroup(conditions_learner_->parameters(),
                         "conditions learner");
    }
    /** Sets splitting quality object.
     */
    void setSplittingQuality(SplittingQuality::Ptr splitting_quality) {
      splitting_quality_ = splitting_quality;
      removeParametersGroup("splitting quality");
      addParametersGroup(splitting_quality_->parameters(),
                         "splitting quality");
    }

    void setDefaultParameters() {
      this->addIntParameter("MIN_VERTEX_SIZE", 3);
      this->addDoubleParameter("LABEL_EPS", 0.001);
    }
    void checkParameters() const {
      CHECK_INT_PARAMETER("MIN_VERTEX_SIZE", X > 0);
      CHECK_DOUBLE_PARAMETER("LABEL_EPS", X >= 0);
    }

    DecisionTreeScorer makeImpl() const {
      return scorer_;
    }
    void setInitialScorer(const DecisionTreeScorer& scorer) {
      scorer_ = scorer;
    }
    void reset() {
      scorer_ = DecisionTreeScorer();
    }

    explicit DecisionTreeLearner(
      const ParametersContainer& parameters = ParametersContainer());
};
}
}
#endif  // LTR_LEARNERS_DECISION_TREE_DECISION_TREE_LEARNER_H_
