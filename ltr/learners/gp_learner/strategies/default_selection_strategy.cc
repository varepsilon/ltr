// Copyright 2012 Yandex

#include "ltr/learners/gp_learner/strategies/default_selection_strategy.h"

namespace ltr {
namespace gp {
void DefaultSelectionStrategy::
  HandlePopulation(vector<Tree>& population, Context& context) { // NOLINT
    Puppy::applySelectionTournament(population, context,
                                    number_of_participants_);
}
}
}
