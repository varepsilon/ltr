// Copyright 2012 Yandex

#include "ltr/learners/gp_learner/strategies/default_mutation_standard_strategy.h"

namespace ltr {
namespace gp {
void DefaultMutationStandardStrategy::
  handlePopulation(vector<Tree>& population, Context& context) { // NOLINT
    this->checkParameters();

    Puppy::applyMutationStandard(population, context, mutation_probability_,
                                 max_regeneration_depth_, max_depth_);
}

void DefaultMutationStandardStrategy::setDefaultParameters() {
  mutation_probability_ = 0.05;
  max_regeneration_depth_ = 5;
  max_depth_ = 35;
}

void DefaultMutationStandardStrategy::checkParameters() const {
  CHECK(0.0 <= mutation_probability_ && mutation_probability_ <= 1.0);
  CHECK(max_regeneration_depth_ > 0);
  CHECK(max_depth_ > 0);
}

void DefaultMutationStandardStrategy::
  setParametersImpl(const ParametersContainer& parameters) {
    mutation_probability_ = parameters.Get<double>("MUTATION_PROBABILITY");
    max_regeneration_depth_ = parameters.Get<int>("MAX_REGENERATION_DEPTH");
    max_depth_ = parameters.Get<int>("MAX_DEPTH");
}
};
};
