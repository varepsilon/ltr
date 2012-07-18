// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_GP_LEARNER_CROSSOVER_ADJACENT_TREES_STRATEGY_H_
#define LTR_LEARNERS_GP_LEARNER_CROSSOVER_ADJACENT_TREES_STRATEGY_H_

#include <vector>

#include "contrib/puppy/Puppy.hpp"
#include "ltr/learners/gp_learner/strategies/population_handler.h"

using std::vector;

using Puppy::Tree;
using Puppy::Context;

namespace ltr {
namespace gp {
/**
* Crossover all adjacent trees in population.
*/
class CrossoverAdjacentTreesStrategy : public BasePopulationHandler {
 public:
  explicit CrossoverAdjacentTreesStrategy(double distribution_probability = 0.3,
                                          int max_depth = 35)
  : distribution_probability_(distribution_probability),
    max_depth_(max_depth) {}

  virtual void HandlePopulation(vector<Tree>& population, Context& context);

 private:
   double distribution_probability_;
   int max_depth_;
};

void CrossoverAdjacentTreesStrategy::
  HandlePopulation(vector<Tree>& population, Context& context) {
  for (int tree_index = 0;
       tree_index < (int)population.size(); tree_index += 2) {
    Puppy::mateTrees(population[tree_index], population[tree_index + 1],
                     context, distribution_probability_, max_depth_);
  }
}
}
}
#endif  // LTR_LEARNERS_GP_LEARNER_CROSSOVER_ADJACENT_TREES_STRATEGY_H_
