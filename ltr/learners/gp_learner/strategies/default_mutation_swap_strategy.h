// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_GP_LEARNER_DEFAULT_MUTATION_SWAP_STRATEGY_H_
#define LTR_LEARNERS_GP_LEARNER_DEFAULT_MUTATION_SWAP_STRATEGY_H_

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
* Puppy applyMutationSwap function.
*/
class DefaultMutationSwapStrategy : public BasePopulationHandler {
 public:
  explicit DefaultMutationSwapStrategy(double mutation_probability = 0.05,
                                       double distribution_probability = 0.5)
  : mutation_probability_(mutation_probability),
    distribution_probability_(distribution_probability) {}

  virtual void HandlePopulation(vector<Tree>& population, Context& context);

 private:
  double mutation_probability_;
  double distribution_probability_;
};

void DefaultMutationSwapStrategy::
  HandlePopulation(vector<Tree>& population, Context& context) {
    Puppy::applyMutationSwap(population, context, mutation_probability_,
                             distribution_probability_);
}
}
}
#endif  // LTR_LEARNERS_GP_LEARNER_DEFAULT_MUTATION_SWAP_STRATEGY_H_
