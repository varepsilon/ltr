// Copyright 2011 Yandex

#ifndef LTR_SCORERS_COMPOSITION_SCORERS_ORDER_STATISTIC_COMPOSITION_SCORER_H_
#define LTR_SCORERS_COMPOSITION_SCORERS_ORDER_STATISTIC_COMPOSITION_SCORER_H_

#include <string>

#include "ltr/scorers/composition_scorers/composition_scorer.h"

using std::string;

namespace ltr {
namespace composition {
/**
 * A composition scorer - contains other scorers with their weights. Scores as
 * weighted order statistic of outputs of its weak scorers. Has a "RATIO" parameter,
 * if ratio = 0 then it scores the minimum of weak scores, if ratio = 1 - the
 * maximum, if ratio = 0.5 - the weighted median. By default ratio = 0.5
 */
class OrderStatisticCompositionScorer : public CompositionScorer {
 public:
  typedef ltr::utility::shared_ptr<OrderStatisticCompositionScorer> Ptr;

  explicit OrderStatisticCompositionScorer(double ratio = 0.5)
    : ratio_(ratio) {}
  /**
   * @param parameters Standart LTR parameter container with double parameter
   * RATIO. By default RATIO = 0.5
   */
  explicit OrderStatisticCompositionScorer
      (const ParametersContainer& parameters) {
    this->setParameters(parameters);
  }

  virtual string toString() const;
  virtual double scoreImpl(const Object& object) const;

  virtual void setDefaultParameters() {
    ratio_ = 0.5;
  }
  /**
   * Checks if RATIO is between 0 and 1 (should be true)
   */
  virtual void checkParameters() const {
    CHECK(0 <= ratio_ <= 1);
  }

  GET_SET(double, ratio);
 private:
  virtual string generateCppCodeImpl(const string& function_name) const;
  virtual string getDefaultAlias() const {
    return "OrderStatisticCompositionScorer";
  }

  virtual void setParametersImpl(const ParametersContainer& parameters) {
    ratio_ = parameters.Get<double>("RATIO");
  }

  double ratio_;
};
};
};

#endif  // LTR_SCORERS_COMPOSITION_SCORERS_ORDER_STATISTIC_COMPOSITION_SCORER_H_
