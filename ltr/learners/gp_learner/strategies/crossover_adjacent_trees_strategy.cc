// Copyright 2012 Yandex

#include "ltr/learners/gp_learner/strategies/crossover_adjacent_trees_strategy.h"

namespace ltr {
namespace gp {
void CrossoverAdjacentTreesStrategy::
  HandlePopulation(vector<Tree>& population, Context& context) { // NOLINT
  for (int tree_index = 0;
       tree_index < (int)population.size(); tree_index += 2) {
    Puppy::mateTrees(population[tree_index], population[tree_index + 1],
                     context, distribution_probability_, max_depth_);
  }
}
}
}
