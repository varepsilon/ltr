// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_GP_LEARNER_STRATEGIES_POPULATION_HANDLER_H_
#define LTR_LEARNERS_GP_LEARNER_STRATEGIES_POPULATION_HANDLER_H_

#include <vector>

#include "ltr/utility/shared_ptr.h"

#include "ltr/interfaces/parameterized.h"

#include "ltr/parameters_container/parameters_container.h"

using std::vector;

using Puppy::Tree;
using Puppy::Context;

namespace ltr {
namespace gp {
/**
 * A base class for population handlers.
 *
 * Handle population of trees for GPLearner.
 */
class BasePopulationHandler : public Parameterized {
 public:
  typedef ltr::utility::shared_ptr<BasePopulationHandler> Ptr;

  virtual void handlePopulation(vector<Tree>& population, Context& context) = 0; // NOLINT
};
};
};
#endif  // LTR_LEARNERS_GP_LEARNER_STRATEGIES_POPULATION_HANDLER_H_
