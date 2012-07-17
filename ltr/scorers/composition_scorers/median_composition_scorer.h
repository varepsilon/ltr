// Copyright 2011 Yandex

#ifndef LTR_SCORERS_COMPOSITION_SCORERS_MEDIAN_COMPOSITION_SCORER_H_
#define LTR_SCORERS_COMPOSITION_SCORERS_MEDIAN_COMPOSITION_SCORER_H_

#include <string>

#include "ltr/scorers/composition_scorers/order_statistic_composition_scorer.h"

using std::string;

namespace ltr {
class MedianCompositionScorer : public OrderStaticticCompositionScorer {
 public:
  typedef ltr::utility::shared_ptr<MedianCompositionScorer> Ptr;

  MedianCompositionScorer()
    : OrderStaticticCompositionScorer(0.5) {}

  virtual string toString() const;

 private:
  virtual string getDefaultAlias() const {
    return "MedianCompositionScorer";
  }
};
};

#endif  // LTR_SCORERS_COMPOSITION_SCORERS_MEDIAN_COMPOSITION_SCORER_H_
