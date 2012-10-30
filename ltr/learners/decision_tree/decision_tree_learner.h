// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_DECISION_TREE_DECISION_TREE_LEARNER_H_
#define LTR_LEARNERS_DECISION_TREE_DECISION_TREE_LEARNER_H_

#include <rlog/rlog.h>

#include <vector>
#include <string>

#include "ltr/learners/learner.h"

#include "ltr/learners/decision_tree/base_splitter.h"
#include "ltr/learners/decision_tree/leaf_generator/leaf_generator.h"
#include "ltr/learners/decision_tree/split_criteria/stop_splitting_criteria.h"
#include "ltr/learners/decision_tree/vertex/decision_vertex.h"
#include "ltr/learners/decision_tree/vertex/leaf_vertex.h"

#include "ltr/scorers/decision_tree_scorer.h"

#include "ltr/utility/shared_ptr.h"

using std::string;
using std::vector;

namespace ltr {
namespace decision_tree {
/**
 * \brief DecisionTreeLearner
 * Builds decision tree for given data.
 */
class DecisionTreeLearner : public BaseLearner<Object, DecisionTreeScorer> {
 public:
  typedef ltr::utility::shared_ptr<DecisionTreeLearner> Ptr;

  explicit DecisionTreeLearner(const ParametersContainer& parameters) {
    this->setParameters(parameters);
  }

  DecisionTreeLearner(BaseSplitter::Ptr splitter,
                      LeafGenerator::Ptr leaf_generator)
  : splitter_(splitter),
    leaf_generator_(leaf_generator) {}

  string toString() const;


  GET_SET(BaseSplitter::Ptr, splitter);
  GET_SET(LeafGenerator::Ptr, leaf_generator);
  GET_SET_VECTOR_OF_PTR(StopSplittingCriteria, stop_splitting_criteria);

 private:
  virtual void setParametersImpl(const ParametersContainer& parameters);
  /**
   * Function creates decision tree for given data.
   * Uses BaseSplitter to create it.
   */
  Vertex::Ptr buildTree(const DataSet<Object>& data);

  bool needToStopSplitting(const DataSet<Object>& data) const;

  LeafVertex::Ptr generateLeaf(const DataSet<Object>& data) const;

  void learnImpl(const DataSet<Object>& data, DecisionTreeScorer* scorer);

  virtual string getDefaultAlias() const;

  BaseSplitter::Ptr splitter_;
  LeafGenerator::Ptr leaf_generator_;
  vector<StopSplittingCriteria::Ptr> stop_splitting_criteria_;
};
};
};

#endif  // LTR_LEARNERS_DECISION_TREE_DECISION_TREE_LEARNER_H_
