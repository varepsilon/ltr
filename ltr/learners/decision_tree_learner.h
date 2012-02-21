// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_DECISION_TREE_LEARNER_H_
#define LTR_LEARNERS_DECISION_TREE_LEARNER_H_

#include <vector>

#include "ltr/learners/learner.h"

#include "ltr/scorers/decision_tree_scorer.h"

#include "ltr/decision_tree/utility/utility.h"
#include "ltr/decision_tree/decision_vertex.h"
#include "ltr/decision_tree/leaf_vertex.h"

using ltr::decision_tree::DecisionVertexPtr;
using ltr::decision_tree::LeafVertexPtr;

using ltr::decision_tree::LeafVertex;
using ltr::decision_tree::split;

namespace ltr {

template<class ConditionsLearner, class SplittingQuality>
class DecisionTreeLearner
  : public Learner<Object, DecisionTreeScorer> {
  private:
    DecisionTreeScorer scorer_;
    typename ConditionsLearner::Ptr conditions_learner_;
    typename SplittingQuality::Ptr splitting_quality_;

    Vertex<double>::Ptr createOneVertex(const DataSet<Object>& data) {
      if (data.size() == 1) {
        LeafVertex<double>::Ptr vertex = LeafVertexPtr<double>();
        vertex->setValue(data[0][0].actualLabel());
        return vertex;
      }
      std::cout << "split " << data.size() << std::endl;
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

    void learnImpl(const DataSet<Object>& data) {
      scorer_.setTree(DecisionTree<double>());
      scorer_.setTreeRoot(createOneVertex(data));
    }

  public:
    DecisionTreeScorer make() const {
      return scorer_;
    }
    void setInitialScorer(const DecisionTreeScorer& scorer) {
      scorer_ = scorer;
    }
    void reset() {
      scorer_ = DecisionTreeScorer();
    }

// Realisation starts here------------------------------------------------------
    explicit DecisionTreeLearner
        (const ParametersContainer& parameters) {
      this->setDefaultParameters();
      this->copyParameters(parameters);

      conditions_learner_ =
          ConditionsLearner::Ptr(new ConditionsLearner());
      splitting_quality_ =
          SplittingQuality::Ptr(new SplittingQuality());
    }
};
}
#endif  // LTR_LEARNERS_DECISION_TREE_LEARNER_H_
