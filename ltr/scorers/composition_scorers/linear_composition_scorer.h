// Copyright 2011 Yandex

#ifndef LTR_SCORERS_COMPOSITION_SCORERS_LINEAR_COMPOSITION_SCORER_H_
#define LTR_SCORERS_COMPOSITION_SCORERS_LINEAR_COMPOSITION_SCORER_H_

#include <string>

#include "ltr/scorers/composition_scorers/soft_composition_scorer.h"

using std::string;

namespace ltr {
class LinearCompositionScorer : public SoftCompositionScorer {
 public:
  typedef boost::shared_ptr<LinearCompositionScorer> Ptr;

  virtual const vector<double>& gradient
    (const vector<double>& weak_scores) const;

  virtual string toString() const;
  virtual double scoreImpl(const Object& object) const;

 private:
  virtual string generateCppCodeImpl(const string& function_name) const;
  virtual string getDefaultAlias() const {return "LinearCompositionScorer";}
};
};

#endif  // LTR_SCORERS_COMPOSITION_SCORERS_LINEAR_COMPOSITION_SCORER_H_
