// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_GP_LEARNER_STRATEGIES_DEFAULT_CROSSOVER_STRATEGY_H_
#define LTR_LEARNERS_GP_LEARNER_STRATEGIES_DEFAULT_CROSSOVER_STRATEGY_H_

#include <vector>

#include "contrib/puppy/Puppy.hpp"

#include "ltr/learners/gp_learner/strategies/population_handler.h"
#include "ltr/utility/macros.h"

using std::vector;

using Puppy::Tree;
using Puppy::Context;

namespace ltr {
namespace gp {
/**
 * Crossover random tree pairs with
 * Puppy applyCrossover function.
 */
class DefaultCrossoverStrategy : public BasePopulationHandler {
 public:
  explicit DefaultCrossoverStrategy(double mating_probability = 0.9,
                                    double distribution_probability = 0.5,
                                    int max_depth = 35)
  : mating_probability_(mating_probability),
    distribution_probability_(distribution_probability),
    max_depth_(max_depth) {}

  explicit DefaultCrossoverStrategy(const ParametersContainer& parameters) {
    this->setParameters(parameters);
  }

  virtual void handlePopulation(vector<Tree>& population, Context& context); // NOLINT

  virtual void setDefaultParameters();

  virtual void checkParameters() const;

  GET_SET(double, mating_probability);
  GET_SET(double, distribution_probability);
  GET_SET(int, max_depth);

 private:
  virtual void setParametersImpl(const ParametersContainer& parameters);

  double mating_probability_;
  double distribution_probability_;
  int max_depth_;
};
};
};
#endif  // LTR_LEARNERS_GP_LEARNER_STRATEGIES_DEFAULT_CROSSOVER_STRATEGY_H_
