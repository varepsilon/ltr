// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_GP_LEARNER_SELECT_TOP_TREES_STRATEGY_H_
#define LTR_LEARNERS_GP_LEARNER_SELECT_TOP_TREES_STRATEGY_H_

#include <vector>
#include <algorithm>

#include "contrib/puppy/Puppy.hpp"
#include "ltr/learners/gp_learner/strategies/population_handler.h"

using std::vector;

using Puppy::Tree;
using Puppy::Context;

namespace ltr {
namespace gp {
/**
* Select top fraction of trees for the next generation.
*/
class SelectTopTreesStrategy : public BasePopulationHandler {
 public:
  explicit SelectTopTreesStrategy(double top_fraction = 0.3)
  : top_fraction_(top_fraction) {}

  virtual void HandlePopulation(vector<Tree>& population, Context& context);

 private:
  double top_fraction_;
};

void SelectTopTreesStrategy::
  HandlePopulation(vector<Tree>& population, Context& context)  {
    std::sort(population.rbegin(), population.rend());

    int top_bound_index = ceil(top_fraction_ * population.size());
    for (int tree_index = top_bound_index;
         tree_index < population.size(); ++tree_index) {
      population[tree_index] = population[tree_index % top_bound_index];
    }
}
}
}
#endif  // LTR_LEARNERS_GP_LEARNER_SELECT_TOP_TREES_STRATEGY_H_
