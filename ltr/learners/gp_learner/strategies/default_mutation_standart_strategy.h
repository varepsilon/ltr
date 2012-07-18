// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_GP_LEARNER_STRATEGIES_DEFAULT_MUTATION_STANDART_STRATEGY_H_
#define LTR_LEARNERS_GP_LEARNER_STRATEGIES_DEFAULT_MUTATION_STANDART_STRATEGY_H_

#include <vector>

#include "contrib/puppy/Puppy.hpp"

#include "ltr/learners/gp_learner/strategies/population_handler.h"

using std::vector;

using Puppy::Tree;
using Puppy::Context;

namespace ltr {
namespace gp {
/**
* Mutate all trees in population with
* Puppy applyMutationStandard function.
*/
class DefaultMutationStandartStrategy : public BasePopulationHandler {
 public:
  explicit DefaultMutationStandartStrategy(double mutation_probability = 0.05,
                                           int max_regeneration_depth = 5,
                                           int max_depth = 35)
  : mutation_probability_(mutation_probability),
    max_regeneration_depth_(max_regeneration_depth),
    max_depth_(max_depth) {}

  virtual void HandlePopulation(vector<Tree>& population, Context& context); // NOLINT

 private:
  double mutation_probability_;
  int max_regeneration_depth_;
  int max_depth_;
};
}
}
#endif  // LTR_LEARNERS_GP_LEARNER_STRATEGIES_DEFAULT_MUTATION_STANDART_STRATEGY_H_
