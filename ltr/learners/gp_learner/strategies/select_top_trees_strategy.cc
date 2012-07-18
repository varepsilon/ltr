// Copyright 2012 Yandex

#include "ltr/learners/gp_learner/strategies/select_top_trees_strategy.h"

namespace ltr {
namespace gp {
void SelectTopTreesStrategy::
  HandlePopulation(vector<Tree>& population, Context& context) { // NOLINT
    std::sort(population.rbegin(), population.rend());

    int top_bound_index = ceil(top_fraction_ * population.size());
    for (int tree_index = top_bound_index;
         tree_index < population.size(); ++tree_index) {
      population[tree_index] = population[tree_index % top_bound_index];
    }
}
}
}
