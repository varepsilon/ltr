//  Copyright 2013 yandex

#ifndef LTR_OPTIMIZATION_STOP_CRITERIA_ITERATION_STOP_CRITERIA_H_
#define LTR_OPTIMIZATION_STOP_CRITERIA_ITERATION_STOP_CRITERIA_H_

#include "ltr/utility/macros.h"
#include "ltr/utility/numerical.h"
#include "ltr/utility/boost/shared_ptr.h"
#include "ltr/optimization/stop_criteria/one_point_stop_criterion.h"

namespace optimization {
typedef Eigen::VectorXd Point;
typedef Eigen::VectorXd Vector;
typedef Eigen::MatrixXd Matrix;
/**
 * \brief Class for test iteration of point from population.
 *
 * Stop criterion is true when current iteration is
 * more than max_iteration.
 *
 * \tparam TFunction is a type of function.
 *
 * \see OnePointStopCriterion, PerPointStopCriterion.
 */
template<class TFunction> class IterationCountStopCriterion
  : public OnePointStopCriterion<TFunction> {
 public:
  typedef ltr::utility::shared_ptr< IterationCountStopCriterion<TFunction> > Ptr;

  explicit IterationCountStopCriterion(typename TFunction::Ptr function,
                                       int max_iteration = 1000)
    : OnePointStopCriterion<TFunction>(function),
      max_iteration_(max_iteration) { }

  ~IterationCountStopCriterion() { }

  /**
   * Init data for future test of current point.
   */
  void init(const Point& point);
  /**
   * Update data for future test of current point.
   */
  void update(const Point& point);

  GET_SET(int, max_iteration)
  GET_SET(int, current_iteration)

  string getDefaultAlias() const {
   return "IterationCountStopCriterion";
  }
  /**
   * Create deep copy of this stop criterion.
   */
  typename OnePointStopCriterion<TFunction>::Ptr clone() const;
 private:
  int max_iteration_;
  int current_iteration_;
};

/**
 * \brief Class for test delta function of point from population.
 *
 * Stop criterion is true when difference between
 * current and previous  value in point.
 *
 * \tparam TFunction is a type of function.
 *
 * \see OnePointStopCriterion, PerPointStopCriterion.
 */
template<class TFunction> class DeltaFunctionStopCriterion
  : public OnePointStopCriterion<TFunction> {
 public:
  typedef ltr::utility::shared_ptr< DeltaFunctionStopCriterion<TFunction> > Ptr;

  explicit DeltaFunctionStopCriterion(typename TFunction::Ptr function,
                                      double min_delta = 0.01)
    : OnePointStopCriterion<TFunction>(function),
      min_delta_(min_delta) { }

  ~DeltaFunctionStopCriterion() { }

  /**
   * Init data for future test of current point.
   */
  void init(const Point& point);
  /**
   * Update data for future test of current point.
   */
  void update(const Point& point);

  GET_SET(double, current_value);
  GET_SET(double, min_delta);

  string getDefaultAlias() const {
    return "DeltaFunctionStopCriterion";
  }
  /**
   * Create deep copy of this stop criterion.
   */
  typename OnePointStopCriterion<TFunction>::Ptr clone() const;
 private:
  double current_value_;
  double min_delta_;
};

/**
 * \brief Class for test delta argument of point from population.
 *
 * Stop criterion is true when distance between current
 * and previous point less than delta_argument.
 *
 * \tparam TFunction is a type of function.
 *
 * \see OnePointStopCriterion, PerPointStopCriterion.
 */
template<class TFunction> class DeltaArgumentStopCriterion
  : public OnePointStopCriterion<TFunction> {
 public:
  typedef ltr::utility::shared_ptr<DeltaArgumentStopCriterion> Ptr;

  explicit DeltaArgumentStopCriterion(typename TFunction::Ptr function,
                                      double min_delta = 0.01)
    : OnePointStopCriterion<TFunction>(function),
      min_delta_(min_delta) { }

  ~DeltaArgumentStopCriterion() { }

  /**
   * Init data for future test of current point.
   */
  void init(const Point& point);
  /**
   * Update data for future test of current point.
   */
  void update(const Point& point);

  GET_SET(Point, current_argument)
  GET_SET(double, min_delta)

  string getDefaultAlias() const { return "DeltaArgumentStopCriterion"; }
  /**
   * Create deep copy of this stop criterion.
   */
  typename OnePointStopCriterion<TFunction>::Ptr clone() const;
 private:
  Point current_argument_;
  double min_delta_;
};

/**
 * \brief Class for test gradient of function
 * in current point from population.
 *
 * Stop criterion is true when gradient in point
 * less than min_gradient.
 *
 * \tparam TFunction is a type of function.
 *
 * \see OnePointStopCriterion, PerPointStopCriterion.
 */
template<class TFunction> class GradientMagnitudeStopCriterion
  : public OnePointStopCriterion<TFunction> {
 public:
  typedef ltr::utility::shared_ptr<GradientMagnitudeStopCriterion> Ptr;

  explicit GradientMagnitudeStopCriterion(typename TFunction::Ptr function,
                                          double min_gradient = 0.01)
    : OnePointStopCriterion<TFunction>(function),
      min_gradient_(min_gradient) { }

  ~GradientMagnitudeStopCriterion() { }

  /**
   * Init data for future test of current point.
   */
  void init(const Point& point);
  /**
   * Update data for future test of current point.
   */
  void update(const Point& point);

  GET_SET(double, min_gradient)

  string getDefaultAlias() const { return "GradientMagnitudeStopCriterion"; }
  /**
   * Create deep copy of this stop criterion.
   */
  typename OnePointStopCriterion<TFunction>::Ptr clone() const;
 private:
  double min_gradient_;
};
//  template realizaation

template<class TFunction>
void IterationCountStopCriterion<TFunction>::init(const Point& point) {
  current_iteration_ = 0;
}
template<class TFunction>
void IterationCountStopCriterion<TFunction>::update(const Point& point) {
  current_iteration_++;  
  this->set_is_true(current_iteration_ >= max_iteration_);
}
template<class TFunction>
typename OnePointStopCriterion<TFunction>::Ptr
    IterationCountStopCriterion<TFunction>::clone() const {
  typename IterationCountStopCriterion<TFunction>::Ptr stop_criterion =
    new IterationCountStopCriterion<TFunction>(this->function_,
                                               max_iteration_);
  stop_criterion->set_current_iteration(this->current_iteration());
  stop_criterion->set_is_true(this->isTrue());
  return stop_criterion;
}

template<class TFunction>
void DeltaFunctionStopCriterion<TFunction>::init(const Point& point) {
  current_value_ = this->function_->computeValue(point);
}
template<class TFunction>
void DeltaFunctionStopCriterion<TFunction>::update(const Point& point) {
  double new_value = this->function_->computeValue(point);
  this->set_is_true(fabs(current_value_ - new_value) < min_delta_);
  current_value_ = this->function_->computeValue(point);
}
template<class TFunction>
typename OnePointStopCriterion<TFunction>::Ptr 
    DeltaFunctionStopCriterion<TFunction>::clone() const {
  typename DeltaFunctionStopCriterion<TFunction>::Ptr stop_criterion =
    new DeltaFunctionStopCriterion<TFunction>(this->function_, min_delta_);
  stop_criterion->set_is_true(this->isTrue());
  stop_criterion->set_current_value(current_value());
  return stop_criterion;
}

template<class TFunction>
void DeltaArgumentStopCriterion<TFunction>::init(const Point& point) {
  current_argument_ = point;
}
template<class TFunction>
void DeltaArgumentStopCriterion<TFunction>::update(const Point& point) {
  this->set_is_true((current_argument_ - point).norm() < min_delta_);
  current_argument_ = point;
}
template<class TFunction>
typename OnePointStopCriterion<TFunction>::Ptr 
    DeltaArgumentStopCriterion<TFunction>::clone() const {
  typename DeltaArgumentStopCriterion<TFunction>::Ptr stop_criterion =
    new DeltaArgumentStopCriterion<TFunction>(this->function_, min_delta_);
  stop_criterion->set_is_true(this->isTrue());
  stop_criterion->set_current_argument(this->current_argument());
  return stop_criterion;
}

template<class TFunction>
void GradientMagnitudeStopCriterion<TFunction>::init(const Point& point) { }
template<class TFunction>
void GradientMagnitudeStopCriterion<TFunction>::update(const Point& point) {
  Vector gradient;
  this->function_->computeGradient(point, &gradient);
  this->set_is_true(gradient.norm() < min_gradient_);
}

template<class TFunction>
typename OnePointStopCriterion<TFunction>::Ptr 
    GradientMagnitudeStopCriterion<TFunction>::clone() const {
    typename GradientMagnitudeStopCriterion<TFunction>::Ptr stop_criterion =
      new GradientMagnitudeStopCriterion<TFunction>(this->function_, min_gradient_);
    stop_criterion->set_is_true(this->isTrue());
    return stop_criterion;
  }
}
#endif  // LTR_OPTIMIZATION_STOP_CRITERIA_ITERATION_STOP_CRITERIA_H_
