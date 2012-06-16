// Copyright 2012 Yandex

#include <iostream>
#include <algorithm>
#include <functional>

#include "ltr/learners/decision_tree/decision_tree_learner.h"

using std::stringstream;

namespace ltr {
namespace decision_tree {

DecisionTreeLearner::DecisionTreeLearner(const ParametersContainer& parameters) {
  this->setDefaultParameters();
  this->copyParameters(parameters);

  setConditionsLearner(
      ConditionsLearner::Ptr(new FakeConditionsLearner()));
  setSplittingQuality(
      SplittingQuality::Ptr(new FakeSplittingQuality()));
}

Vertex<double>::Ptr DecisionTreeLearner::createOneVertex(
    const DataSet<Object>& data) {
  INFO("Creating vertex for %d objects.", data.size());
  if (data.size() == 0) {
    throw std::logic_error("no objects given to decision tree learner");
  }
  Vertex<double>::Ptr vertex = DecisionVertexPtr<double>();
  conditions_learner_->setDataSet(data);
  double best_quality;
  vector<Condition::Ptr> best_conditions;
  vector<DataSet<Object> > datas;
  vector<Condition::Ptr> conditions;
  double min_label = 1e9, max_label = -1e9;
  bool generate_leaf = 0;

  for (int i = 1; i < data.size(); i++) {
    min_label = std::min(min_label, data[i].actual_label());
    max_label = std::max(max_label, data[i].actual_label());
  }

  const ParametersContainer &params = this->parameters();
  if (max_label - min_label <= params.Get<double>("LABEL_EPS")) {
    INFO("All objects has the same label. Leaf vertex created.");
    generate_leaf = 1;
  }
  if (!generate_leaf &&
       data.size() <= params.Get<int>("MIN_VERTEX_SIZE")) {
    INFO("Objects count is less than %d. Leaf vertex created.",
         params.Get<int>("MIN_VERTEX_SIZE"));
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
    LeafVertex<double>::Ptr vertex = LeafVertexPtr<double>();
    double average = 0;
    double sum = 0;
    for (int i = 0; i < data.size(); i++) {
      average += data[i].actual_label() * data.getWeight(i);
      sum += data.getWeight(i);
    }
    if (sum != 0)
      average /= sum;
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
  for (size_t i = 0; i < datas.size(); i++)
    if (datas[i].size() != 0)
      sizes << datas[i].size() << " ";
  INFO("Sizes: %s", sizes.str().c_str());
  for (size_t i = 0; i < datas.size(); i++) {
    if (datas[i].size() != 0) {
      Vertex<double>::Ptr tmp = createOneVertex(datas[i]);
      tmp->setCondition(best_conditions[i]);
      vertex->addChild(tmp);
    }
  }
  return vertex;
}
}
}
