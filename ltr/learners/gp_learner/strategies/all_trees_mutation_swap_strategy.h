// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_GP_LEARNER_ALL_TREES_MUTATION_SWAP_STRATEGY_H_
#define LTR_LEARNERS_GP_LEARNER_ALL_TREES_MUTATION_SWAP_STRATEGY_H_

#include <vector>

#include "contrib/puppy/Puppy.hpp"
#include "ltr/learners/gp_learner/strategies/population_handler.h"

namespace ltr {
namespace gp {
class AllTreesMutationSwapStrategy : public BasePopulationHandler {
 public:
  explicit AllTreesMutationSwapStrategy(double mutation_probability = 0.5,
                                        double distribution_probability = 0.5)
  : mutation_probability_(mutation_probability),
    distribution_probability_(distribution_probability) {}

  virtual void HandlePopulation(std::vector<Puppy::Tree>& population,
                                 Puppy::Context& context) {
    for (int tree_index = 0;
         tree_index < (int)population.size(); ++tree_index) {
        if (context.mRandom.rollUniform() <= mutation_probability_) {
          Puppy::mutateSwap(population[tree_index], context,
                            distribution_probability_);
        }
    }
  }

 private:
   double mutation_probability_;
   double distribution_probability_;
};
}
}
#endif  // LTR_LEARNERS_GP_LEARNER_ALL_TREES_MUTATION_SWAP_STRATEGY_H_
