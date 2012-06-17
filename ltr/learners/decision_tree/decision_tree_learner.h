// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_DECISION_TREE_DECISION_TREE_LEARNER_H_
#define LTR_LEARNERS_DECISION_TREE_DECISION_TREE_LEARNER_H_

#include <vector>
#include <string>
#include <functional>

#include "ltr/learners/learner.h"

#include "ltr/scorers/decision_tree_scorer.h"

#include "ltr/learners/decision_tree/utility/utility.h"
#include "ltr/learners/decision_tree/decision_vertex.h"
#include "ltr/learners/decision_tree/leaf_vertex.h"

#include "ltr/learners/decision_tree/splitting_quality.h"
#include "ltr/learners/decision_tree/conditions_learner.h"

#include "contrib/logog/include/logog.hpp"

using std::string;

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
    INFO("Learn started. Data set size: %d" , data.size());
    scorer_.setTree(DecisionTree<double>());
    scorer_.setTreeRoot(createOneVertex(data));
  }
  virtual string getDefaultAlias() const {return "DecisionTreeLearner";}

 public:
  /** Sets conditiona learner object.
   */
  void setConditionsLearner(ConditionsLearner::Ptr conditions_learner) {
    conditions_learner_ = conditions_learner;
    this->setExistingParameter("conditions learner",
                        conditions_learner_->parameters());
  }
  /** Sets splitting quality object.
   */
  void setSplittingQuality(SplittingQuality::Ptr splitting_quality) {
    splitting_quality_ = splitting_quality;
    this->setExistingParameter("splitting quality",
                        splitting_quality_->parameters());
  }

  void setDefaultParameters() {
    this->addNewParam("MIN_VERTEX_SIZE", 3);
    this->addNewParam("LABEL_EPS", 0.001);
  }
  void checkParameters() const {
    this->checkParameter<int>("MIN_VERTEX_SIZE",
                                       std::bind2nd(std::greater<int>(), 0) );
    this->checkParameter<double>("LABEL_EPS",
                            std::bind2nd(std::greater_equal<double>(), 0) );
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
