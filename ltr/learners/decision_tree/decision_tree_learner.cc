// Copyright 2012 Yandex

#include <iostream>
#include <algorithm>
#include <functional>

#include "ltr/learners/decision_tree/decision_tree_learner.h"

using std::stringstream;

namespace ltr {
namespace decision_tree {
void DecisionTreeLearner::setDefaultParameters() {
  min_vertex_size_ = 3;
  label_eps_ = 0.001;
}

void DecisionTreeLearner::checkParameters() const {
  CHECK(min_vertex_size_ > 0);
  CHECK(label_eps_ >= 0);
}

DecisionTreeLearner::
  DecisionTreeLearner(const ParametersContainer& parameters) {
    this->setParameters(parameters);
    set_conditions_learner(ConditionsLearner::Ptr(new FakeConditionsLearner()));
    set_splitting_quality(SplittingQuality::Ptr(new FakeSplittingQuality()));
}

DecisionTreeLearner::
  DecisionTreeLearner(int min_vertex_size, double label_eps) {
    min_vertex_size_ = min_vertex_size;
    label_eps_ = label_eps;
    set_conditions_learner(ConditionsLearner::Ptr(new FakeConditionsLearner()));
    set_splitting_quality(SplittingQuality::Ptr(new FakeSplittingQuality()));
}

void DecisionTreeLearner::
  setParametersImpl(const ParametersContainer& parameters) {
    INFO("Setting parameters of DecisionTreeLearner.");

    min_vertex_size_ = parameters.Get<int>("MIN_VERTEX_SIZE");
    label_eps_ = parameters.Get<double>("LABEL_EPS");
    conditions_learner_ =
      parameters.Get<ConditionsLearner::Ptr>("CONDITIONS_LEARNER");
    splitting_quality_ =
      parameters.Get<SplittingQuality::Ptr>("SPLITTING_QUALITY");
}

bool DecisionTreeLearner::
  needToGenerateLeaf(const DataSet<Object>& data) const {
    double min_label = 1e9;
    double max_label = -1e9;

    for (int object_index = 0;
         object_index < (int)data.size(); object_index++) {
      min_label = std::min(min_label, data[object_index].actual_label());
      max_label = std::max(max_label, data[object_index].actual_label());
    }

    if (max_label - min_label <= label_eps_) {
      INFO("All objects has the same label. Leaf vertex created.");
      return true;
    }

    if (data.size() <= min_vertex_size_) {
      INFO("Objects count is less than %d. Leaf vertex created.",
        min_vertex_size_);
      return true;
    }

    conditions_learner_->setDataSet(data);
    vector<Condition::Ptr> temp_conditions;
    if (conditions_learner_->getNextConditions(&temp_conditions) == 0) {
      INFO("Can't generate any splits for data set. Leaf vertex created.");
      return true;
    }

    return false;
}

LeafVertex<double>::Ptr DecisionTreeLearner::
  generateLeaf(const DataSet<Object>& data) const {
    INFO("Leaf was generated.");

    LeafVertex<double>::Ptr leaf = LeafVertexPtr<double>();

    double average = 0;
    double sum = 0;
    for (int object_index = 0;
         object_index < (int)data.size(); ++object_index) {
        average +=
          data[object_index].actual_label() * data.getWeight(object_index);
        sum += data.getWeight(object_index);
    }

    if (sum != 0) {
      average /= sum;
    }
    leaf->setValue(average);

    return leaf;
}

void DecisionTreeLearner::
  buildNextLayer(const vector<VertexWithData>& current_layer,
                       vector<VertexWithData>* next_layer) {
    for (int layer_index = 0;
         layer_index < (int)current_layer.size(); ++layer_index) {
      Vertex<double>::Ptr vertex = current_layer[layer_index].vertex;
      const DataSet<Object>& data = current_layer[layer_index].data;

      double best_quality = -1e9;
      vector<Condition::Ptr> best_conditions;
      vector<DataSet<Object> > datas;
      vector<Condition::Ptr> conditions;

      conditions_learner_->setDataSet(data);
      while (conditions_learner_->getNextConditions(&conditions)) {
        split(data, conditions, &datas);
        double quality = splitting_quality_->value(data, datas);
        if (quality > best_quality) {
          best_quality = quality;
          best_conditions = conditions;
        }
      }

      split(data, best_conditions, &datas);
      INFO("Data set splitted into %d sets.", datas.size());

      stringstream sizes;
      for (int data_index = 0; data_index < (int)datas.size(); ++data_index) {
        if (datas[data_index].size() != 0) {
          sizes << datas[data_index].size() << " ";
        }
      }

      INFO("Sizes: %s", sizes.str().c_str());
      for (int data_index = 0; data_index < (int)datas.size(); ++data_index) {
        if (datas[data_index].size() != 0) {
          if (needToGenerateLeaf(datas[data_index])) {
            LeafVertex<double>::Ptr leaf = generateLeaf(datas[data_index]);
            leaf->setCondition(best_conditions[data_index]);
            vertex->addChild(leaf);
          } else {
            Vertex<double>::Ptr tmp = DecisionVertexPtr<double>();
            tmp->setCondition(best_conditions[data_index]);
            vertex->addChild(tmp);
            next_layer->push_back(VertexWithData(tmp, datas[data_index]));
          }
        }
      }
    }
}

Vertex<double>::Ptr DecisionTreeLearner::
  buildTree(const DataSet<Object>& data) {
    if (data.size() == 0) {
      INFO("Data is empty.");
      throw std::logic_error("no objects given to decision tree learner");
    }

    Vertex<double>::Ptr root = DecisionVertexPtr<double>();

    vector<VertexWithData> current_layer;
    current_layer.push_back(VertexWithData(root, data));

    while (!current_layer.empty()) {
      vector<VertexWithData> next_layer;
      buildNextLayer(current_layer, &next_layer);
      current_layer = next_layer;
    }

    return root;
}

void DecisionTreeLearner::
  learnImpl(const DataSet<Object>& data, DecisionTreeScorer* scorer) {
    INFO("Learn started. Data set size: %d" , data.size());
    scorer->setTree(DecisionTree<double>());
    scorer->setTreeRoot(buildTree(data));
}

string DecisionTreeLearner::getDefaultAlias() const {
  return "DecisionTreeLearner";
}
};
};
