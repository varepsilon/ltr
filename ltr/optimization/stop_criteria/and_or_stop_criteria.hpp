//  Copyright 2013 yandex

#ifndef LTR_OPTIMIZATION_STOP_CRITERIA_AND_OR_STOP_CRITERION_H_
#define LTR_OPTIMIZATION_STOP_CRITERIA_AND_OR_STOP_CRITERION_H_

#include <string>
#include "ltr/utility/macros.h"
#include "ltr/utility/numerical.h"
#include "ltr/utility/boost/shared_ptr.h"
#include "ltr/optimization/stop_criteria/one_point_stop_criterion.h"

namespace optimization {
/**
 * \brief Class for test 2 stop criteria
 * in current point from population.
 *
 * Stop criterion is true when both of stop criteria
 * is true in current point.
 *
 * \tparam is a type of function.
 *
 * \see OnePointStopCriterion, PerPointStopCriterion
 */
template<class TFunction>
class And : public OnePointStopCriterion<TFunction> {
 public:
  typedef ltr::utility::shared_ptr<And> Ptr;

  explicit And(typename TFunction::Ptr function,
               typename OnePointStopCriterion<TFunction>::Ptr first,
               typename OnePointStopCriterion<TFunction>::Ptr second);
  ~And() { }

  /**
   * Init data for future test of current point.
   */
  void init(const Point& point);
  /**
   * Update data for future test of current point.
   */
  void update(const Point& point);

  string getDefaultAlias() const;
  /**
   * Create deep copy of this stop criterion.
   */
  typename OnePointStopCriterion<TFunction>::Ptr clone() const;
 private:
  typename OnePointStopCriterion<TFunction>::Ptr first_;
  typename OnePointStopCriterion<TFunction>::Ptr second_;
};

/**
 * \brief Class for test 2 stop criteria
 * in current point from population.
 *
 * Stop criterion is true when one of stop criteria
 * is true in current point.
 *
 * \see OnePointStopCriterion, PerPointStopCriterion
 */
template<class TFunction>
class Or : public OnePointStopCriterion<TFunction> {
 public:
  typedef ltr::utility::shared_ptr<Or> Ptr;

  explicit Or(typename TFunction::Ptr function,
              typename OnePointStopCriterion<TFunction>::Ptr first,
              typename OnePointStopCriterion<TFunction>::Ptr second);
  ~Or() { }

  /**
   * Init data for future test of current point.
   */
  void init(const Point& point);
  /**
   * Update data for future test of current point.
   */
  void update(const Point& point);

  string getDefaultAlias() const;

  typename OnePointStopCriterion<TFunction>::Ptr clone() const;
 private:
  typename OnePointStopCriterion<TFunction>::Ptr first_;
  typename OnePointStopCriterion<TFunction>::Ptr second_;
};

//  template realization

template<class TFunction>
And<TFunction>::And(typename TFunction::Ptr function,
                    typename OnePointStopCriterion<TFunction>::Ptr first,
                    typename OnePointStopCriterion<TFunction>::Ptr second)
    : OnePointStopCriterion<TFunction>(function),
      first_(first),
      second_(second) {
  first_->set_function(function);
  second_->set_function(function);
}
template<class TFunction>
void And<TFunction>::init(const Point& point) {
  first_->init(point);
  second_->init(point);
}
template<class TFunction>
void And<TFunction>::update(const Point& point) {
  first_->update(point);
  second_->update(point);
  set_is_true(first_->isTrue() && second_->isTrue());
}
template<class TFunction>
string And<TFunction>::getDefaultAlias() const {
  return first_->alias() + "_&&_" + second_->alias();
}
template<class TFunction>
typename OnePointStopCriterion<TFunction>::Ptr And<TFunction>::clone() const {
  typename And<TFunction>::Ptr stop_criterion =
    new And<TFunction>(this->function_,
                       first_->clone(),
                       second_->clone());
  stop_criterion->set_is_true(this->isTrue());
  return stop_criterion;
}

template<class TFunction>
Or<TFunction>::Or(typename TFunction::Ptr function,
                  typename OnePointStopCriterion<TFunction>::Ptr first,
                  typename OnePointStopCriterion<TFunction>::Ptr second)
    : OnePointStopCriterion<TFunction>(function),
      first_(first),
      second_(second) {
  first_->set_function(function);
  second_->set_function(function);
}
template<class TFunction>
void Or<TFunction>::init(const Point& point) {
  first_->init(point);
  second_->init(point);
}
template<class TFunction>
void Or<TFunction>::update(const Point& point) {
  first_->update(point);
  second_->update(point);
  set_is_true(first_->isTrue() || second_->isTrue());
}
template<class TFunction>
string Or<TFunction>::getDefaultAlias() const {
  return first_->alias() + "_||_" + second_->alias();
}
template<class TFunction>
typename OnePointStopCriterion<TFunction>::Ptr Or<TFunction>::clone() const {
  typename Or<TFunction>::Ptr stop_criterion =
    new  Or<TFunction>(this->function_,
                       first_->clone(),
                       second_->clone());
  stop_criterion->set_is_true(this->isTrue());
  return stop_criterion;
}
}
#endif  // LTR_OPTIMIZATION_STOP_CRITERIA_AND_OR_STOP_CRITERION_H_
