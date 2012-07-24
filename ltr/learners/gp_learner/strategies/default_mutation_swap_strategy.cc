// Copyright 2012 Yandex

#include "ltr/learners/gp_learner/strategies/default_mutation_swap_strategy.h"

namespace ltr {
namespace gp {
void DefaultMutationSwapStrategy::
  handlePopulation(vector<Tree>& population, Context& context) { // NOLINT
    this->checkParameters();

    Puppy::applyMutationSwap(population, context, mutation_probability_,
                             distribution_probability_);
}

void DefaultMutationSwapStrategy::setDefaultParameters() {
  mutation_probability_ = 0.05;
  distribution_probability_ = 0.5;
}

void DefaultMutationSwapStrategy::checkParameters() const {
  CHECK(0.0 <= mutation_probability_ && mutation_probability_ <= 1.0);
  CHECK(0.0 <= distribution_probability_ && distribution_probability_ <= 1.0);
}

void DefaultMutationSwapStrategy::
  setParametersImpl(const ParametersContainer& parameters) {
    mutation_probability_ = parameters.Get<double>("MUTATION_PROBABILITY");
    distribution_probability_ =
      parameters.Get<double>("DISTRIBUTION_PROBABILITY");
}
};
};
