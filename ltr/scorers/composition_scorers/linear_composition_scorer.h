// Copyright 2011 Yandex

#ifndef LTR_SCORERS_COMPOSITION_SCORERS_LINEAR_COMPOSITION_SCORER_H_
#define LTR_SCORERS_COMPOSITION_SCORERS_LINEAR_COMPOSITION_SCORER_H_

#include <string>
#include <vector>

#include "ltr/scorers/composition_scorers/composition_scorer.h"

using std::string;
using std::vector;

namespace ltr {
namespace composition {
/**
 * A composition scorer - contains other scorers with their weights. Scores as
 * weighted linear composition of outputs of its weak scorers
 */
class LinearCompositionScorer : public CompositionScorer {
 public:
  typedef ltr::utility::shared_ptr<LinearCompositionScorer> Ptr;

  LinearCompositionScorer() {}
  /**
   * @param parameters Standart LTR parameter container with no parameters
   */
  explicit LinearCompositionScorer(const ParametersContainer& parameters) {}

  virtual string toString() const;
  virtual double scoreImpl(const Object& object) const;

 private:
  virtual string generateCppCodeImpl(const string& function_name) const;
  virtual string getDefaultAlias() const {return "LinearCompositionScorer";}
};
};
};

#endif  // LTR_SCORERS_COMPOSITION_SCORERS_LINEAR_COMPOSITION_SCORER_H_
