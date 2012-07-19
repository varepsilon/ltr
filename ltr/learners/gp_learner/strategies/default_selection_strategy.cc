// Copyright 2012 Yandex

#include "ltr/learners/gp_learner/strategies/default_selection_strategy.h"

namespace ltr {
namespace gp {
void DefaultSelectionStrategy::
  handlePopulation(vector<Tree>& population, Context& context) { // NOLINT
    this->checkParameters();

    Puppy::applySelectionTournament(population, context,
                                    number_of_participants_);
}

void DefaultSelectionStrategy::setDefaultParameters() {
  number_of_participants_ = 2;
}

void DefaultSelectionStrategy::checkParameters() const {
  CHECK(number_of_participants_ > 1);
}

void DefaultSelectionStrategy::
  setParametersImpl(const ParametersContainer& parameters) {
    number_of_participants_ = parameters.Get<int>("NUMBER_OF_PARTICIPANTS");
}
}
}
