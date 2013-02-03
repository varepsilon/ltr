// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_DECISION_TREE_DECISION_TREE_LEARNER_H_
#define LTR_LEARNERS_DECISION_TREE_DECISION_TREE_LEARNER_H_

#include <rlog/rlog.h>

#include <vector>
#include <string>
#include <algorithm>
#include "ltr/learners/learner.h"

#include "ltr/learners/decision_tree/base_splitter.h"
#include "ltr/learners/decision_tree/id3_splitter.h"
#include "ltr/learners/decision_tree/leaf_generator/leaf_generator.h"
#include "ltr/learners/decision_tree/leaf_generator/most_common_label_leaf_generator.h"
#include "ltr/learners/decision_tree/split_criteria/stop_splitting_criteria.h"
#include "ltr/learners/decision_tree/vertex/decision_vertex.h"
#include "ltr/learners/decision_tree/vertex/leaf_vertex.h"

#include "ltr/scorers/decision_tree_scorer.h"

#include "ltr/utility/boost/shared_ptr.h"

using std::string;
using std::vector;
using std::swap;

namespace ltr {
namespace decision_tree {
/**
 * \brief DecisionTreeLearner
 * Builds decision tree for given data.
 */
template <class TElement>
class DecisionTreeLearner : public BaseLearner<TElement, DecisionTreeScorer> {
 public:
  typedef ltr::utility::shared_ptr<DecisionTreeLearner> Ptr;

  explicit DecisionTreeLearner(const ParametersContainer& parameters);

  string toString() const;

  DecisionTreeLearner(BaseSplitter::Ptr splitter = new ID3Splitter(),
                      LeafGenerator::Ptr leaf_generator =
                          new MostCommonLabelLeafGenerator())
  : splitter_(splitter),
    leaf_generator_(leaf_generator) {}

  GET_SET(BaseSplitter::Ptr, splitter);
  GET_SET(LeafGenerator::Ptr, leaf_generator);
  GET_SET_VECTOR_OF_PTR(StopSplittingCriteria, stop_splitting_criteria);

 private:
  virtual void setParametersImpl(const ParametersContainer& parameters);

  /**
   * Function creates decision tree for given data.
   * Uses BaseSplitter to create it.
   */
  Vertex::Ptr buildTree(const DataSet<TElement>& data);

  bool tryToSplit(const VertexWithData& vertex_to_split,
                  vector<VertexWithData>& split_result);

  bool needToStopSplitting(const DataSet<Object>& data) const;

  LeafVertex::Ptr generateLeaf(const DataSet<Object>& data) const;

  void learnImpl(const DataSet<TElement>& data, DecisionTreeScorer* scorer);

  virtual string getDefaultAlias() const;

  BaseSplitter::Ptr splitter_;
  LeafGenerator::Ptr leaf_generator_;
  vector<StopSplittingCriteria::Ptr> stop_splitting_criteria_;
};

template<class TElement>
DecisionTreeLearner<TElement>::
  DecisionTreeLearner(const ParametersContainer& parameters) {
    this->setParameters(parameters);
  }

template<class TElement>
string DecisionTreeLearner<TElement>::toString() const {
  return "Decision tree learner with " + splitter_->toString();
}

template<class TElement>
void DecisionTreeLearner<TElement>::
  setParametersImpl(const ParametersContainer& parameters) {
    splitter_ = parameters.Get<BaseSplitter::Ptr>("SPLITTER");
    leaf_generator_ = parameters.Get<LeafGenerator::Ptr>("LEAF_GENERATOR");
    stop_splitting_criteria_ =
      parameters.Get<vector<StopSplittingCriteria::Ptr> >(
        "STOP_SPLITTING_CRITERIAS");
}

template<class TElement>
Vertex::Ptr DecisionTreeLearner<TElement>::
  buildTree(const DataSet<TElement>& data) {
  if (data.size() == 0) {
    rError("Data is empty.");
    throw std::logic_error("no objects given to decision tree learner");
  }

  DataSet<Object> object_data;
  for (int element_index = 0;
       element_index < data.size(); ++element_index) {
       PerObjectAccessor<const TElement>
          per_object_accessor(&data[element_index]);
        for (int object_index = 0;
             object_index < per_object_accessor.object_count();
             ++object_index) {
              object_data.add(per_object_accessor.object(object_index));
        }
  }

  Vertex::Ptr root = new DecisionVertex;
  vector<VertexWithData> current_layer;
  current_layer.push_back(VertexWithData(root, object_data));

  while (!current_layer.empty()) {
    vector<VertexWithData> need_to_split, next_layer, vertex_split_result;
    for (int vertex_index = 0;
       vertex_index < (int)current_layer.size(); ++vertex_index) {
      if (needToStopSplitting(current_layer[vertex_index].data)) {
        LeafVertex::Ptr leaf = generateLeaf(current_layer[vertex_index].data);
        current_layer[vertex_index].vertex->addChild(leaf, new FakeCondition());
      } else if (tryToSplit(current_layer[vertex_index], vertex_split_result)) {
       // if vertex can't be splitted, it should become a leaf
        LeafVertex::Ptr leaf = generateLeaf(current_layer[vertex_index].data);
        current_layer[vertex_index].vertex->addChild(leaf, new FakeCondition());
      } else {  // we should split the vertex
        next_layer.insert(next_layer.end(),
                          vertex_split_result.begin(),
                          vertex_split_result.end());
      }
    }
    if (next_layer.empty()) {
      break;
    }
    swap(current_layer, next_layer);
  }
  return root;
}

template<class TElement>
bool DecisionTreeLearner<TElement>::
  tryToSplit(const VertexWithData& vertex_to_split,
             vector<VertexWithData>& split_result) {
    vector<VertexWithData> need_to_split(1, vertex_to_split);
    splitter_->generateNextLayer(need_to_split, &split_result);
    return need_to_split.size() == split_result.size();
  }

template<class TElement>
bool DecisionTreeLearner<TElement>::
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

template<class TElement>
LeafVertex::Ptr DecisionTreeLearner<TElement>::
  generateLeaf(const DataSet<Object>& data) const {
    return leaf_generator_->operator()(data);
  }

template<class TElement>
void DecisionTreeLearner<TElement>::
  learnImpl(const DataSet<TElement>& data, DecisionTreeScorer* scorer) {
    rInfo("Learn started. Data set size: %d" , data.size());
    DecisionTree::Ptr decision_tree = new DecisionTree(buildTree(data));
    scorer->setTree(decision_tree);
  }

  template<class TElement>
  string DecisionTreeLearner<TElement>::
    getDefaultAlias() const {
      return "DecisionTreeLearner";
  }
};
};

#endif  // LTR_LEARNERS_DECISION_TREE_DECISION_TREE_LEARNER_H_
