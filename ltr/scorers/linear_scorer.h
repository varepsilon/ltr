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
  ALLOW_SHARED_PTR_ONLY_CREATION(LinearScorer)
 public:
  LinearScorer() {}
  explicit LinearScorer(const vector<double>& weights)
  : weight_(weights) {}

  GET_SET_VECTOR(double, weight);

 private:
  virtual double scoreImpl(const Object& obj) const;
  virtual string generateCppCodeImpl(const string& function_name) const;
  virtual string getDefaultAlias() const {return "LinearScorer";}

  string toStringImpl() const;

  vector<double> weight_;
};
};

#endif  // LTR_SCORERS_LINEAR_SCORER_H_
