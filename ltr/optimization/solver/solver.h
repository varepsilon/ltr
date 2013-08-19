// Copyright 2013 Yandex

#ifndef LTR_OPTIMIZATION_SOLVER_SOLVER_H_
#define LTR_OPTIMIZATION_SOLVER_SOLVER_H_

#include <ctime>
#include <limits>
#include <string>

#include "ltr/utility/macros.h"
#include "ltr/optimization/functions/function.h"
#include "ltr/optimization/sets/set.h"
#include "ltr/optimization/population_initers/population_initer.h"
#include "ltr/optimization/population_updaters/population_updater.h"
#include "ltr/optimization/population_info/population.h"
#include "ltr/optimization/stop_criteria/stop_criterion.h"
#include "ltr/optimization/solver/solution.h"
#include "ltr/utility/cross_platform_time.h"

using std::numeric_limits;
using ltr::utility::time;

namespace optimization {

/**
 * \brief A class which solves an optimization task given a function.
 *        a set, a stop criterion, a population updater and population initer.
 */
template<typename TFunction>
class Solver : public ltr::Aliaser {
  public:
    Solver(PopulationIniter::Ptr population_initer,
           typename PopulationUpdater<TFunction>::Ptr population_updater,
           typename StopCriterion<TFunction>::Ptr stop_criterion);
    /**
     * A trigger for solving given optimization task.
     */
    Solution solve(typename TFunction::Ptr function,
                   Set::Ptr set,
                   int population_size) const;

    virtual string getDefaultAlias() const {
      return "Solver";
    }

  private:
    PopulationIniter::Ptr initer_;
    typename PopulationUpdater<TFunction>::Ptr updater_;
    typename StopCriterion<TFunction>::Ptr stop_criterion_;
};

template<typename TFunction>
Solver<TFunction>::Solver(PopulationIniter::Ptr population_initer,
               typename PopulationUpdater<TFunction>::Ptr population_updater,
               typename StopCriterion<TFunction>::Ptr stop_criterion) :
    initer_(population_initer),
    updater_(population_updater),
    stop_criterion_(stop_criterion)
      {  }

template<typename TFunction>
Solution Solver<TFunction>::solve(typename TFunction::Ptr function,
                                  Set::Ptr set,
                                  int population_size) const {
  Population::Ptr population = new Population;
  initer_->populationInit(population_size, *set, population.get());

  stop_criterion_->init(population.get(), function, set);

  updater_->init(population.get(), function, set);

  int iteration_count = 0;
  int start_time = time();

  while (!stop_criterion_->isTrue()) {
    ++iteration_count;
    updater_->update(population.get());
    stop_criterion_->update(*(population.get()));
  }

  int end_time = time();

  double min_value = numeric_limits<double>::max();
  Point argmin = Point::Zero(set->dimension());

  for (Population::Iterator it = population->begin();
       it != population->end();
       ++it) {
    double cur_value = function->computeValue(it.point());
    if (cur_value < min_value) {
      min_value = cur_value;
      argmin = it.point();
    }
  }

  return Solution(min_value, argmin, iteration_count,
                  (long long)(end_time - start_time) * 1000 / CLOCKS_PER_SEC); // NOLINT
}
}

#endif  // LTR_OPTIMIZATION_SOLVER_SOLVER_H_
