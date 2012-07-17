// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_GP_LEARNER_MULTIPLY_TOP_TREES_STRATEGY_H_
#define LTR_LEARNERS_GP_LEARNER_MULTIPLY_TOP_TREES_STRATEGY_H_

#include <vector>
#include <algorithm>

#include "contrib/puppy/Puppy.hpp"
#include "ltr/learners/gp_learner/strategies/population_handler.h"

namespace ltr {
namespace gp {
class MultiplyTopTreesStrategy : public BasePopulationHandler {
 public:
  explicit MultiplyTopTreesStrategy(double top_for_next_generation_part = 0.3)
  : top_for_next_generation_part_(top_for_next_generation_part) {}

  virtual void HandlePopulation(std::vector<Puppy::Tree>& population,
                                 Puppy::Context& context)  {
    std::sort(population.rbegin(), population.rend());

    int top_bound_index = top_for_next_generation_part_ * population.size();

    for (int tree_index = top_bound_index;
         tree_index < top_bound_index; ++tree_index) {
      population[tree_index] = population[tree_index % top_bound_index];
    }
  }
 private:
  double top_for_next_generation_part_;
};
}
}
#endif  // LTR_LEARNERS_GP_LEARNER_MULTIPLY_TOP_TREES_STRATEGY_H_
