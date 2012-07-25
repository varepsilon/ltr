// Copyright 2012 Yandex

#include "ltr/learners/gp_learner/strategies/default_crossover_strategy.h"

namespace ltr {
namespace gp {
void DefaultCrossoverStrategy::
  handlePopulation(vector<Tree>& population, Context& context) { // NOLINT
    this->checkParameters();

    Puppy::applyCrossover(population, context, mating_probability_,
                          distribution_probability_, max_depth_);
}

void DefaultCrossoverStrategy::setDefaultParameters() {
  mating_probability_ = 0.9;
  distribution_probability_ = 0.5;
  max_depth_ = 35;
}

void DefaultCrossoverStrategy::checkParameters() const {
  CHECK(0.0 <= mating_probability_ && mating_probability_ <= 1.0);
  CHECK(0.0 <= distribution_probability_ && distribution_probability_ <= 1.0);
  CHECK(max_depth_ > 0);
}

void DefaultCrossoverStrategy::
  setParametersImpl(const ParametersContainer& parameters) {
    mating_probability_ = parameters.Get<double>("MATING_PROBABILITY");
    distribution_probability_ =
      parameters.Get<double>("DISTRIBUTION_PROBABILITY");
    max_depth_ = parameters.Get<int>("MAX_DEPTH");
}
};
};
