// Copyright 2011 Yandex

#ifndef LTR_SCORERS_LINEAR_SCORER_H_
#define LTR_SCORERS_LINEAR_SCORER_H_

#include <Eigen/Dense>
#include <string>

#include "ltr/scorers/scorer.h"

using Eigen::VectorXd;

namespace ltr {
class LinearScorer : public Scorer {
 public:
  typedef boost::shared_ptr<LinearScorer> Ptr;

  explicit LinearScorer(const VectorXd& in_weights)
    : Scorer("Linear scorer"), weights(in_weights) {}

  string brief() const;
 private:
  virtual double scoreImpl(const Object& obj) const;
  virtual string generateCppCodeImpl(const string& function_name) const;

  VectorXd weights;
};
};

#endif  // LTR_SCORERS_LINEAR_SCORER_H_
