// Copyright 2013 Yandex

#ifndef LTR_OPTIMIZATION_POPULATION_UPDATERS_DIRECTION_CALCULATORS_HPP_
#define LTR_OPTIMIZATION_POPULATION_UPDATERS_DIRECTION_CALCULATORS_HPP_

#include <string>
#include "ltr/utility/random_number_generator.h"
#include "ltr/optimization/population_updaters/direction_calculator.hpp"

using ltr::utility::randomizer;

namespace optimization {
/** \brief Class for calculate gradient direction from
 * given point to minimize given function.
 *
 * \see StepDirectionCalculator.
 */
template<class TFunction>
class GradientDirectionCalculator : public StepDirectionCalculator<TFunction> {
 public:
  typedef ltr::utility::shared_ptr<GradientDirectionCalculator> Ptr;
  explicit GradientDirectionCalculator() { }
  ~GradientDirectionCalculator() { }

  virtual void getStepDirection(const Point& point, Vector* direction);
  virtual typename StepDirectionCalculator<TFunction>::Ptr clone();
  virtual string getDefaultAlias() const {
    return "GradientDirectionCalculator";
  }
};

/** \brief Class for calculate conjugate gradient direction from
 * given point to minimize given function.
 *
 * \see StepDirectionCalculator.
 */
template<class TFunction>
class ConjugateDirectionCalculator
    : public StepDirectionCalculator<TFunction> {
 public:
  typedef ltr::utility::shared_ptr<ConjugateDirectionCalculator> Ptr;
  explicit ConjugateDirectionCalculator()
     : is_first_iteration_(true) { }
  ~ConjugateDirectionCalculator() { }

  virtual void getStepDirection(const Point& point, Vector* direction);
  virtual typename StepDirectionCalculator<TFunction>::Ptr clone();
  virtual string getDefaultAlias() const {
    return "StochasticDirectionCalculator";
  }
  GET_SET(typename StepSizeCalculator<TFunction>::Ptr, step_size_calculator);
 private:
  typename StepSizeCalculator<TFunction>::Ptr step_size_calculator_;
  bool is_first_iteration_;
  Vector antigradient_;
  Vector previous_direction_;
};

// template realization

template<class TFunction>
void GradientDirectionCalculator<TFunction>::getStepDirection(
    const Point& point,
    Vector* direction) {
  this->function_->computeGradient(point, direction);
  *direction = -*direction;
}
template<class TFunction>
typename StepDirectionCalculator<TFunction>::Ptr GradientDirectionCalculator<TFunction>::clone() {  // NOLINT
  typename GradientDirectionCalculator<TFunction>::Ptr
    cloned_direction_calculator =
      new GradientDirectionCalculator();
  cloned_direction_calculator->set_function(this->function_);
  return cloned_direction_calculator;
}

template<class TFunction>
void ConjugateDirectionCalculator<TFunction>::getStepDirection(
    const Point& point,
    Vector* direction) {
  if (is_first_iteration_) {
    this->function_->computeGradient(point, &antigradient_);
    antigradient_ *= -1.0;
    *direction = antigradient_;
    previous_direction_ = antigradient_;
    is_first_iteration_ = false;
  } else {
     Vector antigradient;
     this->function_->computeGradient(point, &antigradient);
     antigradient *= -1.0;

    double w = (antigradient.dot(antigradient - antigradient_)) /
                 (antigradient_.dot(antigradient_));
    if (w < 0.0) {
      w = 0.0;
    }
    antigradient_ = antigradient;
    *direction = antigradient + w * previous_direction_;
    if (this->step_size_calculator()->getStepSize(point, *direction) != 0.0) {
      previous_direction_ = *direction;
    } else {
      *direction = previous_direction_;
    }
  }
}

template<class TFunction>
typename StepDirectionCalculator<TFunction>::Ptr ConjugateDirectionCalculator<TFunction>::clone() {  // NOLINT
  typename ConjugateDirectionCalculator<TFunction>::Ptr
    cloned_direction_calculator =
      new ConjugateDirectionCalculator();
  cloned_direction_calculator->set_function(this->function_);
  cloned_direction_calculator->set_step_size_calculator(
    this->step_size_calculator());
  return cloned_direction_calculator;
}
}
#endif  // LTR_OPTIMIZATION_POPULATION_UPDATERS_DIRECTION_CALCULATORS_HPP_
