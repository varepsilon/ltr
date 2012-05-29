// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_LINEAR_LEARNER_LINEAR_LEARNER_H_
#define LTR_LEARNERS_LINEAR_LEARNER_LINEAR_LEARNER_H_

#include <boost/shared_ptr.hpp>
#include <Eigen/Dense>

#include <vector>

#include "ltr/learners/learner.h"
#include "ltr/scorers/linear_scorer.h"

using std::vector;
using Eigen::VectorXd;
using Eigen::MatrixXd;

namespace ltr {
template<class TElement>
class LinearLearner : public Learner<TElement, LinearScorer> {
 public:
  typedef boost::shared_ptr<LinearLearner> Ptr;

  LinearLearner(const ParametersContainer& parameters = ParametersContainer())
      : Learner<TElement, LinearScorer>("LinearScorer") {
    this->setDefaultParameters();
    this->copyParameters(parameters);
  }

  void reset() {}
  void setInitialScorer(const LinearScorer& in_scorer) {}

  LinearScorer makeImpl() const;
 private:
  void learnImpl(const DataSet<TElement>& data);
  // min (Y - Xb)^T W (Y - Xb)
  // b = (X^T W X)^-1 X^T W Y
  VectorXd b;
};


template<class TElement>
LinearScorer LinearLearner<TElement>::makeImpl() const {
  vector<double> weights(b.size());
  for (int i = 0; i < b.size(); ++i) {
    weights[i] = b[i];
  }
  return LinearScorer(weights);
}

template<class TElement>
void LinearLearner<TElement>::learnImpl(const DataSet<TElement>& data) {
  VectorXd Y(data.size());
  for (int i = 0; i < Y.size(); ++i) {
    Y(i) = data[i].actual_label();
  }

  MatrixXd X(data.size(), data.featureCount() + 1);
  for (int object_index = 0; object_index < data.size(); ++object_index) {
    X(object_index, 0) = 1.0;
    for (int feature_index = 0;
        feature_index < data.featureCount(); ++feature_index) {
      X(object_index, feature_index + 1) =
        data[object_index].features()[feature_index];
    }
  }

  // XTW = X^T W
  MatrixXd XTW = X.transpose();
  for (int i = 0; i < data.size(); ++i) {
    XTW.col(i) *= data.getWeight(i);
  }

  b = (XTW * X).ldlt().solve(XTW * Y);
}
}
#endif  // LTR_LEARNERS_LINEAR_LEARNER_LINEAR_LEARNER_H_
