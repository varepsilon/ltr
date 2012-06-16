// Copyright 2011 Yandex

#ifndef LTR_SCORERS_LINEAR_SCORER_H_
#define LTR_SCORERS_LINEAR_SCORER_H_

#include <string>
#include <vector>

#include "ltr/scorers/scorer.h"

using std::string;
using std::vector;

namespace ltr {
class LinearScorer : public Scorer {
 public:
  typedef boost::shared_ptr<LinearScorer> Ptr;

  explicit LinearScorer(const vector<double>& in_weights)
  : weights(in_weights) {}

  string toString() const;
 private:
  virtual double scoreImpl(const Object& obj) const;
  virtual string generateCppCodeImpl(const string& function_name) const;

  vector<double> weights;
};
};

#endif  // LTR_SCORERS_LINEAR_SCORER_H_
