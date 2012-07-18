// Copyright 2012 Yandex

#include "ltr/learners/gp_learner/strategies/default_mutation_swap_strategy.h"

namespace ltr {
namespace gp {
void DefaultMutationSwapStrategy::
  HandlePopulation(vector<Tree>& population, Context& context) { // NOLINT
    Puppy::applyMutationSwap(population, context, mutation_probability_,
                             distribution_probability_);
}
}
}
