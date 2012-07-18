// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_LINEAR_LEARNER_LINEAR_LEARNER_H_
#define LTR_LEARNERS_LINEAR_LEARNER_LINEAR_LEARNER_H_

#include "ltr/utility/shared_ptr.h"
#include <Eigen/Dense>

#include <string>
#include <vector>

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
  VectorXd Y(data.size());
  for (int i = 0; i < Y.size(); ++i) {
    Y(i) = data[i].actual_label();
  }

  MatrixXd X(data.size(), data.feature_count() + 1);
  for (int object_index = 0; object_index < data.size(); ++object_index) {
    X(object_index, 0) = 1.0;
    for (int feature_index = 0;
        feature_index < data.feature_count(); ++feature_index) {
      X(object_index, feature_index + 1) =
        data[object_index][feature_index];
    }
  }

  // XTW = X^T W
  MatrixXd XTW = X.transpose();
  for (int i = 0; i < data.size(); ++i) {
    XTW.col(i) *= data.getWeight(i);
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
