// Copyright 2012 Yandex

#include "ltr/learners/gp_learner/strategies/default_mutation_standart_strategy.h"

namespace ltr {
namespace gp {
  void DefaultMutationStandartStrategy::
    HandlePopulation(vector<Tree>& population, Context& context) { // NOLINT
      Puppy::applyMutationStandard(population, context, mutation_probability_,
                                   max_regeneration_depth_, max_depth_);
  }
};
}
