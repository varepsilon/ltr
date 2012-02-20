// Copyright 2012 Yandex

#include <iostream>

#include "ltr/learners/decision_tree_learner.h"
#include "ltr/decision_tree/utility/utility.h"
#include "ltr/decision_tree/decision_vertex.h"
#include "ltr/decision_tree/leaf_vertex.h"

using ltr::decision_tree::DecisionVertexPtr;
using ltr::decision_tree::LeafVertexPtr;

using ltr::decision_tree::LeafVertex;
using ltr::decision_tree::split;

#include "ltr/learners/utility/id3_splitter.h"

namespace ltr {
  DecisionTreeLearner::DecisionTreeLearner
      (const ParametersContainer& parameters) {
    this->setDefaultParameters();
    this->copyParameters(parameters);

    conditions_learner_ =
        ConditionsLearner::Ptr(new decision_tree::ID3_Splitter());
    splitting_quality_ =
        SplittingQuality::Ptr(new decision_tree::SqrErrorQuality());
  }

  void DecisionTreeLearner::learnImpl(const DataSet<Object>& data) {
    scorer_.setTree(DecisionTree<double>());
    scorer_.setTreeRoot(createOneVertex(data));
  }

  Vertex<double>::Ptr
  DecisionTreeLearner::createOneVertex(const DataSet<Object>& data) {
    if (data.size() == 1) {
      LeafVertex<double>::Ptr vertex = LeafVertexPtr<double>();
      vertex->setValue(data[0][0].actualLabel());
      return vertex;
    }
    std::cout << "split " << data.size() << std::endl;
    if (data.size() == 2)
      int n = 0;
    Vertex<double>::Ptr vertex = DecisionVertexPtr<double>();
    conditions_learner_->setDataSet(data);
    double best_quality;
    vector<Condition::Ptr> best_conditions;
    vector<DataSet<Object> > datas;
    vector<Condition::Ptr> conditions;

    if (conditions_learner_->getNextConditions(&best_conditions)) {
      split(data, best_conditions, &datas);
      best_quality = splitting_quality_->value(data, datas);
    } else {
      throw std::logic_error("ConditionsLearner doesn't work");
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
    for (size_t i = 0; i < datas.size(); i++)
      if (datas[i].size() != 0) {
        Vertex<double>::Ptr tmp = createOneVertex(datas[i]);
        tmp->setCondition(best_conditions[i]);
        vertex->addChild(tmp);
      }

    return vertex;
  }
}
