// Copyright 2013 Yandex

#ifndef LTR_OPTIMIZATION_POPULATION_UPDATERS_DIRECTED_UPDATERS_HPP_
#define LTR_OPTIMIZATION_POPULATION_UPDATERS_DIRECTED_UPDATERS_HPP_

#include <string>
#include "ltr/optimization/population_updaters/directed_updater.hpp"
#include "ltr/optimization/population_updaters/step_size_calculators.hpp"
#include "ltr/optimization/population_updaters/direction_calculators.hpp"

namespace optimization {
/**
 * \brief Class for update one point from population
 * independently on other points via gradient descent with fixed step method.
 *
 * \see DirectedUpdater
 */
template<class TFunction>
class FixedStepGradientUpdater : public DirectedUpdater<TFunction> {
 public:
  explicit FixedStepGradientUpdater(double step)
      : DirectedUpdater<TFunction>(
            new GradientDirectionCalculator<TFunction>(),
            new ConstantStepCalculator<TFunction>(step)) { }
  ~FixedStepGradientUpdater() { }

  virtual string getDefaultAlias() const {
    return "FixedStepGradientUpdater";
  }
};
/**
 * \brief Class for update one point from population
 * independently on other points via gradient descent with variable step method.
 *
 * \see DirectedUpdater
 */
template<class TFunction>
class SteepestGradientUpdater : public DirectedUpdater<TFunction> {
 public:
  SteepestGradientUpdater(double max_step,
                          double brute_force_step)
      : DirectedUpdater<TFunction>(
            new GradientDirectionCalculator<TFunction>(),
            new BruteForceStepCalculator<TFunction>(max_step,
                                                    brute_force_step)) { }
  ~SteepestGradientUpdater() { }
  virtual string getDefaultAlias() const {
    return "SteepestGradientUpdater";
  }
};
/**
 * \brief Class for update one point from population
 * independently on other points via conjugate gradient
 * descent with variable step method.
 *
 * \see DirectedUpdater
 */
template<class TFunction>
class ConjugateGradientUpdater
    : public DirectedUpdater<TFunction> {
 public:
  ConjugateGradientUpdater(double max_step,
                           double brute_force_step)
      : DirectedUpdater<TFunction>(NULL,
                                   new BruteForceStepCalculator<TFunction>(
                                   max_step,
                                   brute_force_step)) {
    typename ConjugateDirectionCalculator<TFunction>::Ptr
      conjugate_gradient_calculator =
        new ConjugateDirectionCalculator<TFunction>();
    conjugate_gradient_calculator->set_step_size_calculator(
       this->step_size_calculator());
    this->set_step_direction_calculator(conjugate_gradient_calculator);
  }
  ~ConjugateGradientUpdater() { }
  virtual string getDefaultAlias() const {
    return "ConjugateGradientUpdater";
  }
};
}

#endif  // LTR_OPTIMIZATION_POPULATION_UPDATERS_DIRECTED_UPDATERS_HPP_
