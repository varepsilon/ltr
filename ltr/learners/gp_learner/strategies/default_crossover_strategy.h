// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_GP_LEARNER_DEFAULT_CROSSOVER_STRATEGY_H_
#define LTR_LEARNERS_GP_LEARNER_DEFAULT_CROSSOVER_STRATEGY_H_

#include <vector>

#include "contrib/puppy/Puppy.hpp"
#include "ltr/learners/gp_learner/strategies/population_handler.h"

namespace ltr {
namespace gp {
class DefaultCrossoverStrategy : public BasePopulationHandler {
 public:
  explicit DefaultCrossoverStrategy(double mating_probability = 0.9,
                                    double distribution_probability = 0.5,
                                    int max_depth = 35)
  : mating_probability_(mating_probability),
    distribution_probability_(distribution_probability),
    max_depth_(max_depth) {}

  virtual void HandlePopulation(std::vector<Puppy::Tree>& population,
                                 Puppy::Context& context) {
    Puppy::applyCrossover(population, context, mating_probability_,
                          distribution_probability_, max_depth_);
  }

 private:
  double mating_probability_;
  double distribution_probability_;
  int max_depth_;
};
}
}
#endif  // LTR_LEARNERS_GP_LEARNER_DEFAULT_CROSSOVER_STRATEGY_H_
