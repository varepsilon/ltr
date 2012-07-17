// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_GP_LEARNER_DEFAULT_MUTATION_SWAP_STRATEGY_H_
#define LTR_LEARNERS_GP_LEARNER_DEFAULT_MUTATION_SWAP_STRATEGY_H_

#include <vector>

#include "contrib/puppy/Puppy.hpp"
#include "ltr/learners/gp_learner/strategies/population_handler.h"

namespace ltr {
namespace gp {
class DefaultMutationSwapStrategy : public BasePopulationHandler {
 public:
  explicit DefaultMutationSwapStrategy(double mutation_probability = 0.05,
                                       int max_regeneration_depth = 5,
                                       int max_depth = 35)
  : mutation_probability_(mutation_probability),
    max_regeneration_depth_(max_regeneration_depth),
    max_depth_(max_depth) {}

  virtual void HandlePopulation(std::vector<Puppy::Tree>& population,
                                 Puppy::Context& context) {
    Puppy::applyMutationStandard(population, context, mutation_probability_,
                                 max_regeneration_depth_, max_depth_);
  }

 private:
  double mutation_probability_;
  int max_regeneration_depth_;
  int max_depth_;
};
}
}
#endif  // LTR_LEARNERS_GP_LEARNER_DEFAULT_MUTATION_SWAP_STRATEGY_H_
