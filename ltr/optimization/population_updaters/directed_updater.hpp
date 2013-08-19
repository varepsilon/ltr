// Copyright 2013 Yandex

#ifndef LTR_OPTIMIZATION_POPULATION_UPDATERS_DIRECTED_UPDATER_HPP_
#define LTR_OPTIMIZATION_POPULATION_UPDATERS_DIRECTED_UPDATER_HPP_

#include <string>
#include "ltr/utility/macros.h"
#include "ltr/interfaces/aliaser.h"
#include "ltr/optimization/population_updaters/one_point_updater.h"
#include "ltr/optimization/population_updaters/direction_calculator.hpp"
#include "ltr/optimization/population_updaters/step_size_calculator.hpp"
#include "ltr/optimization/sets/set.h"

namespace optimization {
/**
 * \brief Abstract class for update one point from population
 * independently on other points via one of gradient methods.
 *
 * You need to define StepDirectionCalculator and StepSizeCalculator.
 *
 * \see OnePointUpdater, StepDirectionCalculator, StepSizeCalculator
 */
template<class TFunction>
class DirectedUpdater : public OnePointUpdater<TFunction> {
 public:
  typedef typename ltr::utility::shared_ptr<DirectedUpdater> Ptr;

  explicit DirectedUpdater(
      typename StepDirectionCalculator<TFunction>::Ptr step_direction_calculator,  // NOLINT
      typename StepSizeCalculator<TFunction>::Ptr step_size_calculator)
      :  step_direction_calculator_(step_direction_calculator),
         step_size_calculator_(step_size_calculator) { }
  ~DirectedUpdater() { }
  /**
   * Init data for future updates.
   */
  virtual void init(const Point& point,
                    typename TFunction::Ptr function,
                    Set::Ptr set);
  /**
   * \brief Update point.
   * You need to define function and set after it.
   */
  virtual void update(const Point& point, Point* updated_point);

  virtual string getDefaultAlias() const {
    return "DirectedUpdater";
  }
  virtual typename OnePointUpdater<TFunction>::Ptr clone() const;
  GET_SET(typename StepDirectionCalculator<TFunction>::Ptr, step_direction_calculator);  // NOLINT
  GET_SET(typename StepSizeCalculator<TFunction>::Ptr, step_size_calculator);  // NOLINT
 private:
  typename StepDirectionCalculator<TFunction>::Ptr step_direction_calculator_;
  typename StepSizeCalculator<TFunction>::Ptr step_size_calculator_;
};

// template realization
template<class TFunction>
void DirectedUpdater<TFunction>::update(const Point& point,
                                         Point* updated_point) {
  Vector direction;
  step_direction_calculator_->getStepDirection(point, &direction);
  double step_size =  step_size_calculator_->getStepSize(point, direction);
  *updated_point = point + direction * step_size;
}

template<class TFunction>
typename OnePointUpdater<TFunction>::Ptr DirectedUpdater<TFunction>::clone() const {  // NOLINT
  typename DirectedUpdater<TFunction>::Ptr cloned_direction_stepper =
    new DirectedUpdater<TFunction>(step_direction_calculator_->clone(),
                                    step_size_calculator_->clone());
  return cloned_direction_stepper;
}

template<class TFunction>
void DirectedUpdater<TFunction>::init(const Point& point,
                                      typename TFunction::Ptr function,
                                      Set::Ptr set) {
  this->set_function(function);
  this->set_set(set);
  step_direction_calculator_->set_function(function);
  step_size_calculator_->set_function(function);
}
}

#endif  // LTR_OPTIMIZATION_POPULATION_UPDATERS_DIRECTED_UPDATER_HPP_
