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

#include "logog/logog.h"

using std::string;

namespace ltr {
namespace decision_tree {

/**
 \class DecisionTreeLearner
 Builds decision tree for given data.
 */

class DecisionTreeLearner
  : public BaseLearner<Object, DecisionTreeScorer> {
 public:
  void setDefaultParameters() {
    min_vertex_size_ = 3;
    label_eps_ = 0.001;
  }

  void checkParameters() const {
    CHECK(min_vertex_size_ > 0); // NOLINT
    CHECK(label_eps_ >= 0); // NOLINT
  }

  GET_SET(ConditionsLearner::Ptr, conditions_learner);
  GET_SET(SplittingQuality::Ptr, splitting_quality);
  GET_SET(int, min_vertex_size);
  GET_SET(double, label_eps);

  explicit DecisionTreeLearner(const ParametersContainer& parameters);
  public:
    string toString() const;

  explicit DecisionTreeLearner(
    int min_vertex_size = 3, double label_eps = 0.001);

 private:
  virtual void setParametersImpl(const ParametersContainer& parameters) {
    min_vertex_size_ = parameters.Get<int>("MIN_VERTEX_SIZE");
    label_eps_ = parameters.Get<double>("LABEL_EPS");
    conditions_learner_
      = parameters.Get<ConditionsLearner::Ptr>("CONDITIONS_LEARNER");
    splitting_quality_
      = parameters.Get<SplittingQuality::Ptr>("SPLITTING_QUALITY");
  }
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

  void learnImpl(const DataSet<Object>& data, DecisionTreeScorer* scorer) {
    INFO("Learn started. Data set size: %d" , data.size());
    scorer->setTree(DecisionTree<double>());
    scorer->setTreeRoot(createOneVertex(data));
  }
  virtual string getDefaultAlias() const {return "DecisionTreeLearner";}

  int min_vertex_size_;
  double label_eps_;
};
}
}
#endif  // LTR_LEARNERS_DECISION_TREE_DECISION_TREE_LEARNER_H_