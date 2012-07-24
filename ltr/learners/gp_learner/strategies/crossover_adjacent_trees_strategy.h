// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_GP_LEARNER_STRATEGIES_CROSSOVER_ADJACENT_TREES_STRATEGY_H_
#define LTR_LEARNERS_GP_LEARNER_STRATEGIES_CROSSOVER_ADJACENT_TREES_STRATEGY_H_

#include <vector>

#include "contrib/puppy/Puppy.hpp"

#include "ltr/learners/gp_learner/strategies/population_handler.h"

using std::vector;

using Puppy::Tree;
using Puppy::Context;

namespace ltr {
namespace gp {
/**
* Crossover all adjacent trees in population.
*/
class CrossoverAdjacentTreesStrategy : public BasePopulationHandler {
 public:
  explicit CrossoverAdjacentTreesStrategy(double distribution_probability = 0.3,
                                          int max_depth = 35)
  : distribution_probability_(distribution_probability),
    max_depth_(max_depth) {}

  explicit CrossoverAdjacentTreesStrategy(
    const ParametersContainer& parameters) {
      this->setParameters(parameters);
  }

  virtual void handlePopulation(vector<Tree>& population, Context& context); // NOLINT

  virtual void setDefaultParameters();

  virtual void checkParameters() const;

  GET_SET(double, distribution_probability);
  GET_SET(int, max_depth);

 private:
  virtual void setParametersImpl(const ParametersContainer& parameters);

  double distribution_probability_;
  int max_depth_;
};
};
};
#endif  // LTR_LEARNERS_GP_LEARNER_STRATEGIES_CROSSOVER_ADJACENT_TREES_STRATEGY_H_
