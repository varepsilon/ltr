// Copyright 2012 Yandex

#include "ltr/learners/gp_learner/strategies/default_crossover_strategy.h"

namespace ltr {
namespace gp {
void DefaultCrossoverStrategy::
  HandlePopulation(vector<Tree>& population, Context& context) { // NOLINT
    Puppy::applyCrossover(population, context, mating_probability_,
                          distribution_probability_, max_depth_);
}
}
}
