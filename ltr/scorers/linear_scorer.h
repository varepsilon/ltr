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
  typedef ltr::utility::shared_ptr<LinearScorer> Ptr;

  LinearScorer() {}
  explicit LinearScorer(const vector<double>& weights)
  : weights_(weights) {}

  double weight(int index) const;
  void set_weight(int index, double weight);
  int weights_count() const;

  string toString() const;
 private:
  virtual double scoreImpl(const Object& obj) const;
  virtual string generateCppCodeImpl(const string& function_name) const;
  virtual string getDefaultAlias() const {return "LinearScorer";}

  vector<double> weights_;
};
};

#endif  // LTR_SCORERS_LINEAR_SCORER_H_
