// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_GP_LEARNER_STRATEGIES_DEFAULT_MUTATION_SWAP_STRATEGY_H_
#define LTR_LEARNERS_GP_LEARNER_STRATEGIES_DEFAULT_MUTATION_SWAP_STRATEGY_H_

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

  explicit DefaultMutationSwapStrategy(const ParametersContainer& parameters) {
    this->setParameters(parameters);
  }

  virtual void handlePopulation(vector<Tree>& population, Context& context); // NOLINT

  virtual void setDefaultParameters();

  virtual void checkParameters() const;

  GET_SET(double, mutation_probability);
  GET_SET(double, distribution_probability);

 private:
  virtual void setParametersImpl(const ParametersContainer& parameters);

  double mutation_probability_;
  double distribution_probability_;
};
}
}
#endif  // LTR_LEARNERS_GP_LEARNER_STRATEGIES_DEFAULT_MUTATION_SWAP_STRATEGY_H_
