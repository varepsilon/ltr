// Copyright 2013 Yandex

#ifndef LTR_OPTIMIZATION_SOLVER_PARTICLE_SWARM_OPTIMIZATION_H_
#define LTR_OPTIMIZATION_SOLVER_PARTICLE_SWARM_OPTIMIZATION_H_

#include "ltr/optimization/solver/solver.h"
#include "ltr/optimization/population_updaters/particle_swarm_updater.h"
#include "ltr/optimization/population_initers/naive_initer.h"
#include "ltr/optimization/stop_criteria/one_point_stop_criteria.hpp"
#include "ltr/optimization/stop_criteria/per_point_stop_criterion.hpp"

namespace optimization {
/**
 * \brief A class which solves an optimization task given a function
 *        using particle swarm optimization algorithm.
 *
 * Get more information about the algorithm on
 * http://sci2s.ugr.es/EAMHCO/pdfs/ABC-algorithm-numerical-function-2007.pdf
 *
 * \tparam TFunction is a type of function.
 *
 * \see Solver
 */
template <class TFunction>
class ParticleSwarmOptimization : public Solver<TFunction> {
 public:
  explicit ParticleSwarmOptimization(
      int max_iteration = 1000,
      double inertia_weight_min = 0.7,
      double inertia_weight_max = 0.9,
      double individual_learning_factor = 2.0,
      double cooperative_learning_factor = 2.0)
      : Solver<TFunction>(new NaiveIniter,
                          new ParticleSwarmUpdater<TFunction>(max_iteration,
                              inertia_weight_min, inertia_weight_max,
                              individual_learning_factor,
                              cooperative_learning_factor),
                          new PerPointStopCriterion<TFunction>(
                              new IterationCountStopCriterion<TFunction>(
                                  max_iteration))) {}
  ~ParticleSwarmOptimization() {}
};
}

#endif  // LTR_OPTIMIZATION_SOLVER_PARTICLE_SWARM_OPTIMIZATION_H_
