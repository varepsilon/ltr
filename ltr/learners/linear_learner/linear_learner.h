// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_LINEAR_LEARNER_LINEAR_LEARNER_H_
#define LTR_LEARNERS_LINEAR_LEARNER_LINEAR_LEARNER_H_

#include <Eigen/Dense>

#include <string>
#include <vector>

#include "ltr/utility/shared_ptr.h"
#include "ltr/learners/learner.h"
#include "ltr/scorers/linear_scorer.h"

using std::string;
using std::vector;

using Eigen::VectorXd;
using Eigen::MatrixXd;

namespace ltr {
template<class TElement>
class LinearLearner : public BaseLearner<TElement, LinearScorer> {
 public:
  typedef ltr::utility::shared_ptr<LinearLearner> Ptr;

  explicit LinearLearner(const ParametersContainer& parameters) {
    // DO NOTHING
  }

  LinearLearner() {
    // DO NOTHING
  }

  // \TODO ? Implement
  void reset() {}
  // \TODO ? Implement
  void setInitialScorer(const LinearScorer& scorer) {}
 private:
  void learnImpl(const DataSet<TElement>& data, LinearScorer* scorer);
  virtual string getDefaultAlias() const {return "LinearLearner";}
};


template<class TElement>
void LinearLearner<TElement>::learnImpl(const DataSet<TElement>& data,
                                        LinearScorer* scorer) {
  rInfo("Learning started");

  DataSet<Object> object_data;

  for (int element_index = 0;
       element_index < data.size();
       ++element_index) {
    PerObjectAccessor<const TElement>
      per_object_accessor(&data[element_index]);
    for (int object_index = 0;
         object_index < per_object_accessor.object_count();
         ++object_index) {
       object_data.add(per_object_accessor.object(object_index));
    }
  }

  VectorXd Y(object_data.size());

  for (int i = 0; i < Y.size(); ++i) {
    Y(i) = object_data[i].actual_label();
  }

  MatrixXd X(object_data.size(), object_data.feature_count() + 1);
  for (int object_index = 0;
      object_index < object_data.size();
      ++object_index) {
    X(object_index, 0) = 1.0;
    for (int feature_index = 0;
        feature_index < object_data.feature_count(); ++feature_index) {
      X(object_index, feature_index + 1) =
        object_data[object_index][feature_index];
    }
  }

  // XTW = X^T W
  rInfo("Calculating XTW matrix");
  MatrixXd XTW = X.transpose();
  for (int i = 0; i < object_data.size(); ++i) {
    XTW.col(i) *= object_data.getWeight(i);
  }
  VectorXd b;
  b = (XTW * X).ldlt().solve(XTW * Y);

  // \TODO rewrite when LinearScorer
  // will have set_weights or set_weight(i, weight)
  vector<double> weights(b.size());
  for (int i = 0; i < b.size(); ++i) {
    weights[i] = b[i];
  }
  *scorer = LinearScorer(weights);
}
}
#endif  // LTR_LEARNERS_LINEAR_LEARNER_LINEAR_LEARNER_H_
