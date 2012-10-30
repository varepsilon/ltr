// Copyright 2012 Yandex

#include "ltr/learners/decision_tree/decision_tree_learner.h"
#include "ltr/learners/decision_tree/condition/condition.h"

using std::stringstream;

namespace ltr {
namespace decision_tree {
string DecisionTreeLearner::toString() const {
  return "Decision tree learner with " + splitter_->toString();
}

void DecisionTreeLearner::
  setParametersImpl(const ParametersContainer& parameters) {
    splitter_ = parameters.Get<BaseSplitter::Ptr>("SPLITTER");
    leaf_generator_ = parameters.Get<LeafGenerator::Ptr>("LEAF_GENERATOR");
    stop_splitting_criteria_ =
      parameters.Get<vector<StopSplittingCriteria::Ptr> >(
        "STOP_SPLITTING_CRITERIAS");
}

Vertex::Ptr DecisionTreeLearner::buildTree(const DataSet<Object>& data) {
  if (data.size() == 0) {
    rError("Data is empty.");
    throw std::logic_error("no objects given to decision tree learner");
  }

  Vertex::Ptr root = new DecisionVertex;
  vector<VertexWithData> current_layer;
  current_layer.push_back(VertexWithData(root, data));

  while (!current_layer.empty()) {
    vector<VertexWithData> need_to_split, next_layer;
    for (int vertex_index = 0;
         vertex_index < (int)current_layer.size(); ++vertex_index) {
      if (needToStopSplitting(current_layer[vertex_index].data)) {
        LeafVertex::Ptr leaf = generateLeaf(current_layer[vertex_index].data);
        current_layer[vertex_index].vertex->addChild(leaf, new FakeCondition());
      } else {
        need_to_split.push_back(current_layer[vertex_index]);
      }
    }
    if (need_to_split.empty()) {
      break;
    }
    splitter_->generateNextLayer(need_to_split, &next_layer);
    swap(current_layer, next_layer);
  }
  return root;
}

bool DecisionTreeLearner::
  needToStopSplitting(const DataSet<Object>& data) const {
    for (int criteria_index = 0;
         criteria_index < (int)stop_splitting_criteria_.size();
         ++criteria_index) {
      if (stop_splitting_criteria_[criteria_index]->
          needToStopSplitting(data)) {
        return true;
      }
    }
    return false;
}

LeafVertex::Ptr DecisionTreeLearner::
  generateLeaf(const DataSet<Object>& data) const {
    return leaf_generator_->operator()(data);
}

void DecisionTreeLearner::
  learnImpl(const DataSet<Object>& data, DecisionTreeScorer* scorer) {
    rInfo("Learn started. Data set size: %d" , data.size());
    DecisionTree::Ptr decision_tree = new DecisionTree(buildTree(data));
    scorer->setTree(decision_tree);
}

string DecisionTreeLearner::getDefaultAlias() const {
  return "DecisionTreeLearner";
}
};
};
