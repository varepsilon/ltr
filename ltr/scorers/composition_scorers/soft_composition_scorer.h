// Copyright 2011 Yandex

#ifndef LTR_SCORERS_COMPOSITION_SCORERS_SOFT_COMPOSITION_SCORER_H_
#define LTR_SCORERS_COMPOSITION_SCORERS_SOFT_COMPOSITION_SCORER_H_

#include <string>
#include <vector>

#include "ltr/scorers/composition_scorers/composition_scorer.h"

using std::string;
using std::vector;

namespace ltr {
class SoftCompositionScorer : public CompositionScorer {
 public:
  typedef boost::shared_ptr<SoftCompositionScorer> Ptr;

  virtual const vector<double>& gradient
    (const vector<double>& weak_scores) const = 0;

 private:
  virtual string getDefaultAlias() const {return "SoftCompositionScorer";}
};
};

#endif  // LTR_SCORERS_COMPOSITION_SCORERS_SOFT_COMPOSITION_SCORER_H_
