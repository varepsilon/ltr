// Copyright 2013 Yandex

#ifndef LTR_OPTIMIZATION_POPULATION_UPDATERS_STEP_SIZE_CALCULATORS_HPP_
#define LTR_OPTIMIZATION_POPULATION_UPDATERS_STEP_SIZE_CALCULATORS_HPP_

#include <string>
#include "ltr/optimization/population_updaters/step_size_calculator.hpp"

namespace optimization {
/** \brief Class for calculate optimzal step size in given direction
 * to minimize given function via brute force method.
 *
 * \see StepSizeCalculator.
 */
template<class TFunction>
class BruteForceStepCalculator : public StepSizeCalculator<TFunction> {
 public:
  typedef ltr::utility::shared_ptr<BruteForceStepCalculator> Ptr;

  explicit BruteForceStepCalculator(double max_step,
                                    double brute_force_step)
    : brute_force_step_(brute_force_step), max_step_(max_step) { }
  ~BruteForceStepCalculator() { }

  virtual double getStepSize(const Point& point, const Vector& direction);
  virtual string getDefaultAlias() const {
    return "BruteForceStepCalculator";
  }
  virtual typename StepSizeCalculator<TFunction>::Ptr clone();
  GET_SET(double, brute_force_step);
  GET_SET(double, max_step);

 private:
  double brute_force_step_;
  double max_step_;
};

/** \brief Class for calculate optimzal step size in given direction
 * to minimize given function which returns constant step.
 *
 * \see StepSizeCalculator.
 */
template<class TFunction>
class ConstantStepCalculator : public StepSizeCalculator<TFunction> {
 public:
  typedef ltr::utility::shared_ptr<ConstantStepCalculator> Ptr;

  explicit ConstantStepCalculator(double step)
    : step_(step) { }
  ~ConstantStepCalculator() { }

  virtual double getStepSize(const Point& point, const Vector& direction);
  virtual string getDefaultAlias() const {
    return "ConstantStepCalculator";
  }

  virtual typename StepSizeCalculator<TFunction>::Ptr clone();
  GET_SET(double, step);

 private:
  double step_;
};

//  template realization
template<class TFunction>
double BruteForceStepCalculator<TFunction>::getStepSize(
    const Point& point,
    const Vector& direction) {
  Point current_point = point;
  double current_value = this->function_->computeValue(current_point);
  double min_value = current_value;
  double best_step = 0.0;
  for (double step = brute_force_step_;
       step <= max_step_;
       step += brute_force_step_) {
    current_point += direction * brute_force_step_;
    current_value = this->function_->computeValue(current_point);

    if (current_value < min_value) {
      min_value = current_value;
      best_step = step;
    }
  }
  return best_step;
}

template<class TFunction>
typename StepSizeCalculator<TFunction>::Ptr BruteForceStepCalculator<TFunction>::clone() {  // NOLINT
  typename BruteForceStepCalculator<TFunction>::Ptr cloned_step_calculator =
    new BruteForceStepCalculator<TFunction>(this->max_step_,
                                            this->brute_force_step_);
  cloned_step_calculator->set_function(this->function_);
  return cloned_step_calculator;
}

template<class TFunction>
double ConstantStepCalculator<TFunction>::getStepSize(const Point& point,
                                                      const Vector& direction) {
  return step_;
}

template<class TFunction>
typename StepSizeCalculator<TFunction>::Ptr ConstantStepCalculator<TFunction>::clone() {  // NOLINT
  typename ConstantStepCalculator<TFunction>::Ptr cloned_step_calculator =
    new ConstantStepCalculator<TFunction>(this->step_);
  cloned_step_calculator->set_function(this->function_);
  return cloned_step_calculator;
}
}
#endif  // LTR_OPTIMIZATION_POPULATION_UPDATERS_STEP_SIZE_CALCULATORS_HPP_
