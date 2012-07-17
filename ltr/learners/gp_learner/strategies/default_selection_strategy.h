// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_GP_LEARNER_DEFAULT_SELECTION_STRATEGY_H_
#define LTR_LEARNERS_GP_LEARNER_DEFAULT_SELECTION_STRATEGY_H_

#include <vector>

#include "contrib/puppy/Puppy.hpp"
#include "ltr/learners/gp_learner/strategies/population_handler.h"

using std::vector;

using Puppy::Tree;
using Puppy::Context;

namespace ltr {
namespace gp {
/**
* Select trees for the next generation with
* Puppy applySelectionTournament function.
*/
class DefaultSelectionStrategy : public BasePopulationHandler {
 public:
  explicit DefaultSelectionStrategy(int number_of_participants = 2)
  : number_of_participants_(number_of_participants) {}

  virtual void HandlePopulation(vector<Tree>& population, Context& context);

 private:
  int number_of_participants_;
};

void DefaultSelectionStrategy::
  HandlePopulation(vector<Tree>& population, Context& context) {
    Puppy::applySelectionTournament(population, context,
                                    number_of_participants_);
}
}
}
#endif  // LTR_LEARNERS_GP_LEARNER_DEFAULT_SELECTION_STRATEGY_H_
