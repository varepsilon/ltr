// Copyright 2011 Yandex

#ifndef LTR_SCORERS_COMPOSITION_SCORERS_MAX_WEIGHT_COMPOSITION_SCORER_H_
#define LTR_SCORERS_COMPOSITION_SCORERS_MAX_WEIGHT_COMPOSITION_SCORER_H_

#include "ltr/scorers/composition_scorers/composition_scorer.h"

namespace ltr {
class MaxWeightCompositionScorer : public CompositionScorer {
 public:
  typedef boost::shared_ptr<MaxWeightCompositionScorer> Ptr;

  virtual string toString() const;
  virtual double scoreImpl(const Object& object) const;

 private:
  virtual string generateCppCodeImpl(const string& function_name) const;
  virtual string getDefaultAlias() const {return "MaxWeightCompositionScorer";}
};
};

#endif  // LTR_SCORERS_COMPOSITION_SCORERS_MAX_WEIGHT_COMPOSITION_SCORER_H_
