// Copyright 2011 Yandex

#ifndef LTR_SCORERS_COMPOSITION_SCORERS_MEDIAN_COMPOSITION_SCORER_H_
#define LTR_SCORERS_COMPOSITION_SCORERS_MEDIAN_COMPOSITION_SCORER_H_

#include <string>

#include "ltr/scorers/composition_scorers/order_statistic_composition_scorer.h"

using std::string;

namespace ltr {
namespace composition {
/**
 * A composition scorer - contains other scorers with their weights. Scores as
 * weighted median of outputs of its weak scorers
 */
class MedianCompositionScorer : public OrderStatisticCompositionScorer {
 public:
  typedef ltr::utility::shared_ptr<MedianCompositionScorer> Ptr;

  MedianCompositionScorer()
    : OrderStatisticCompositionScorer(0.5) {}
  /**
   * @param parameters Standart LTR parameter container with no parameters
   */
  explicit MedianCompositionScorer(const ParametersContainer& parameters) {}

  virtual string toString() const;

 private:
  virtual string getDefaultAlias() const {
    return "MedianCompositionScorer";
  }
};
};
};

#endif  // LTR_SCORERS_COMPOSITION_SCORERS_MEDIAN_COMPOSITION_SCORER_H_
