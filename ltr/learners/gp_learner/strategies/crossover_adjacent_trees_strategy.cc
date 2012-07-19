// Copyright 2012 Yandex

#include "ltr/learners/gp_learner/strategies/crossover_adjacent_trees_strategy.h"

namespace ltr {
namespace gp {
void CrossoverAdjacentTreesStrategy::
  handlePopulation(vector<Tree>& population, Context& context) { // NOLINT
    this->checkParameters();

    for (int tree_index = 0;
         tree_index < (int)population.size(); tree_index += 2) {
      Puppy::mateTrees(population[tree_index], population[tree_index + 1],
                       context, distribution_probability_, max_depth_);
    }
}

void CrossoverAdjacentTreesStrategy::setDefaultParameters() {
  distribution_probability_ = 0.3;
  max_depth_ = 35;
}

void CrossoverAdjacentTreesStrategy::checkParameters() const {
  CHECK(0.0 <= distribution_probability_ && distribution_probability_ <= 1.0);
  CHECK(max_depth_ > 0);
}

void CrossoverAdjacentTreesStrategy::
  setParametersImpl(const ParametersContainer& parameters) {
    distribution_probability_ =
      parameters.Get<double>("DISTRIBUTION_PROBABILITY");
    max_depth_ = parameters.Get<int>("MAX_DEPTH");
}
}
}
