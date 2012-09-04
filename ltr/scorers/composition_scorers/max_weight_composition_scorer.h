// Copyright 2011 Yandex

#ifndef LTR_SCORERS_COMPOSITION_SCORERS_MAX_WEIGHT_COMPOSITION_SCORER_H_
#define LTR_SCORERS_COMPOSITION_SCORERS_MAX_WEIGHT_COMPOSITION_SCORER_H_

#include <string>

#include "ltr/scorers/composition_scorers/composition_scorer.h"

using std::string;

namespace ltr {
namespace composition {
/**
 * A composition scorer - contains other scorers with their weights. Scores as
 * the weak scorer with the largest weight
 */
class MaxWeightCompositionScorer : public CompositionScorer {
  static const int INVALID_INDEX = -1;
 public:
  typedef ltr::utility::shared_ptr<MaxWeightCompositionScorer> Ptr;

  MaxWeightCompositionScorer() : best_index_(INVALID_INDEX) {}
  /**
   * @param parameters Standart LTR parameter container with no parameters
   */
  explicit MaxWeightCompositionScorer(const ParametersContainer& parameters)
    : best_index_(INVALID_INDEX) {}

  virtual void clear();
  virtual string toString() const;
  virtual double scoreImpl(const Object& object) const;

 private:
  virtual void addImpl(const ScorerAndWeight& weighted_scorer);
  virtual string generateCppCodeImpl(const string& function_name) const;
  virtual string getDefaultAlias() const {return "MaxWeightCompositionScorer";}

  int best_index_;
};
};
};

#endif  // LTR_SCORERS_COMPOSITION_SCORERS_MAX_WEIGHT_COMPOSITION_SCORER_H_
