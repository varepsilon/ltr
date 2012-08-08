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

Vertex<double>::Ptr DecisionTreeLearner::
  createOneVertex(const DataSet<Object>& data) {
    INFO("Creating vertex for %d objects.", data.size());

    if (data.size() == 0) {
      INFO("Data is empty.");
      throw std::logic_error("no objects given to decision tree learner");
    }

    Vertex<double>::Ptr vertex = DecisionVertexPtr<double>();
    conditions_learner_->setDataSet(data);

    double best_quality;
    vector<Condition::Ptr> best_conditions;
    vector<DataSet<Object> > datas;
    vector<Condition::Ptr> conditions;
    double min_label = -1e9, max_label = 1e9;
    bool generate_leaf = 0;

    for (int object_index = 1;
         object_index < (int)data.size();
         object_index++) {
      min_label = std::min(min_label, data[object_index].actual_label());
      max_label = std::max(max_label, data[object_index].actual_label());
    }

    if (max_label - min_label <= label_eps_) {
      INFO("All objects has the same label. Leaf vertex created.");
      generate_leaf = 1;
    }
    if (!generate_leaf && data.size() <= min_vertex_size_) {
      INFO("Objects count is less than %d. Leaf vertex created.",
        min_vertex_size_);
      generate_leaf = 1;
    }
    if (!generate_leaf &&
        conditions_learner_->getNextConditions(&best_conditions)) {
      split(data, best_conditions, &datas);
      best_quality = splitting_quality_->value(data, datas);
    } else {
      INFO("Can't generate any splits for data set. Leaf vertex created.");
      generate_leaf = 1;
    }

    if (generate_leaf) {
      INFO("Leaf was generated.");
      LeafVertex<double>::Ptr vertex = LeafVertexPtr<double>();

      double average = 0;
      double sum = 0;
      for (int i = 0; i < (int)data.size(); ++i) {
        average += data[i].actual_label() * data.getWeight(i);
        sum += data.getWeight(i);
      }

      if (sum != 0) {
        average /= sum;
      }
      vertex->setValue(average);
      return vertex;
    }

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
    for (int i = 0; i < (int)datas.size(); ++i) {
      if (datas[i].size() != 0) {
        sizes << datas[i].size() << " ";
      }
    }

    INFO("Sizes: %s", sizes.str().c_str());
    for (int i = 0; i < (int)datas.size(); ++i) {
      if (datas[i].size() != 0) {
        Vertex<double>::Ptr tmp = createOneVertex(datas[i]);
        tmp->setCondition(best_conditions[i]);
        vertex->addChild(tmp);
      }
    }
    return vertex;
}

void DecisionTreeLearner::
  learnImpl(const DataSet<Object>& data, DecisionTreeScorer* scorer) {
    INFO("Learn started. Data set size: %d" , data.size());
    scorer->setTree(DecisionTree<double>());
    scorer->setTreeRoot(createOneVertex(data));
}

string DecisionTreeLearner::getDefaultAlias() const {
  return "DecisionTreeLearner";
}
};
};
