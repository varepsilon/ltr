// Copyright 2011 Yandex

#ifndef LTR_SCORERS_COMPOSITION_SCORERS_ORDER_STATISTIC_COMPOSITION_SCORER_H_
#define LTR_SCORERS_COMPOSITION_SCORERS_ORDER_STATISTIC_COMPOSITION_SCORER_H_

#include <string>

#include "ltr/scorers/composition_scorers/composition_scorer.h"

using std::string;

namespace ltr {
class OrderStaticticCompositionScorer : public CompositionScorer {
 public:
  typedef boost::shared_ptr<OrderStaticticCompositionScorer> Ptr;

  explicit OrderStaticticCompositionScorer(double ratio)
    : ratio_(ratio) {}

  virtual string toString() const;
  virtual double scoreImpl(const Object& object) const;

 private:
  virtual string generateCppCodeImpl(const string& function_name) const;
  virtual string getDefaultAlias() const {
    return "OrderStaticticCompositionScorer";
  }

  double ratio_;
};
};

#endif  // LTR_SCORERS_COMPOSITION_SCORERS_ORDER_STATISTIC_COMPOSITION_SCORER_H_
