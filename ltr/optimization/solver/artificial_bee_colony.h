// Copyright 2013 Yandex

#ifndef LTR_OPTIMIZATION_SOLVER_ARTIFICIAL_BEE_COLONY_H_
#define LTR_OPTIMIZATION_SOLVER_ARTIFICIAL_BEE_COLONY_H_

#include <string>

#include "ltr/optimization/solver/solver.h"
#include "ltr/optimization/population_updaters/bee_colony_updater.h"
#include "ltr/optimization/population_initers/naive_initer.h"
#include "ltr/optimization/stop_criteria/one_point_stop_criteria.hpp"
#include "ltr/optimization/stop_criteria/per_point_stop_criterion.hpp"

namespace optimization {
/**
 * \brief A class which solves an optimization task given a function
 *        using artificial bee colony algorithm.
 *
 * Get more information about the algorithm on
 * http://sci2s.ugr.es/EAMHCO/pdfs/ABC-algorithm-numerical-function-2007.pdf
 *
 * \tparam TFunction is a type of function.
 *
 * \see Solver
 */
template <class TFunction>
class ArtificialBeeColony : public Solver<TFunction> {
 public:
  explicit ArtificialBeeColony(int max_iteration = 1000,
                               int no_update_limit = 1000) :
      Solver<TFunction>(new NaiveIniter,
                        new BeeColonyUpdater<TFunction>(no_update_limit),
                        new PerPointStopCriterion<TFunction>(
                            new IterationCountStopCriterion<TFunction>(
                                max_iteration))) {}
  ~ArtificialBeeColony() {}
  virtual string getDefaultAlias() const {
    return "ArtificialBeeColony";
  }
};
}

#endif  // LTR_OPTIMIZATION_SOLVER_ARTIFICIAL_BEE_COLONY_H_
