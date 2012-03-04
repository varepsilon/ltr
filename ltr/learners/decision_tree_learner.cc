// Copyright 2012 Yandex

#include <iostream>

#include "ltr/learners/decision_tree_learner.h"

namespace ltr {
namespace decision_tree {

DecisionTreeLearner::DecisionTreeLearner(const ParametersContainer& parameters)
    : Learner<Object, DecisionTreeScorer>("DecisionTreeLearner"),
      log(logger::Logger::LL_INFO, "DT_Learner ") {
  this->setDefaultParameters();
  this->copyParameters(parameters.getParametersGroup(""));

  conditions_learner_ =
      ConditionsLearner::Ptr(new FakeConditionsLearner());
  splitting_quality_ =
      SplittingQuality::Ptr(new FakeSplittingQuality());
}

Vertex<double>::Ptr DecisionTreeLearner::createOneVertex(
    const DataSet<Object>& data) {
  log << "Creating vertex for " << data.size() << " objects." << std::endl;
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
    min_label = std::min(min_label, data[i].actualLabel());
    max_label = std::max(max_label, data[i].actualLabel());
  }

  if (max_label - min_label <= this->getDoubleParameter("LABEL_EPS")) {
    log << "All objects has the same label. Leaf vertex created."
        << std::endl;
    generate_leaf = 1;
  }
  if (!generate_leaf &&
       data.size() <= this->getIntParameter("MIN_VERTEX_SIZE")) {
    log << "Objects count is less than "
        << this->getIntParameter("MIN_VERTEX_SIZE")
        << ". Leaf vertex created." << std::endl;
    generate_leaf = 1;
  }
  if (!generate_leaf &&
       conditions_learner_->getNextConditions(&best_conditions)) {
    split(data, best_conditions, &datas);
    best_quality = splitting_quality_->value(data, datas);
  } else {
    log << "Can't generate any splits for data set. Leaf vertex created.";
    generate_leaf = 1;
  }

  if (generate_leaf) {
    LeafVertex<double>::Ptr vertex = LeafVertexPtr<double>();
    double average = 0;
    double sum = 0;
    for (int i = 0; i < data.size(); i++) {
      average += data[i].actualLabel() * data.getWeight(i);
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
  log << "Data set splitted into " << datas.size()
      << " sets. Sizes: ";
  for (size_t i = 0; i < datas.size(); i++)
    if (datas[i].size() != 0)
      log << datas[i].size() << " ";
  log << std::endl;
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
