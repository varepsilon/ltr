// Copyright 2012 Yandex

#ifndef LTR_OPTIMIZATION_STOP_CRITERIA_ONE_POINT_STOP_CRITERION_H_
#define LTR_OPTIMIZATION_STOP_CRITERIA_ONE_POINT_STOP_CRITERION_H_

#include <string>
#include "contrib/include_Eigen.h"
#include "ltr/utility/macros.h"
#include "ltr/interfaces/aliaser.h"
#include "ltr/utility/boost/shared_ptr.h"
#include "ltr/optimization/sets/set.h"

namespace optimization {
typedef Eigen::VectorXd Point;
/**
 * \brief Abtract class for test one point from population
 * independently on other points.
 *
 * \tparam is a type of function.
 *
 * \see StopCriterion, PerPointStopCriterion
 */
template<class TFunction> class OnePointStopCriterion : public ltr::Aliaser {
 public:
  typedef ltr::utility::shared_ptr< OnePointStopCriterion<TFunction> > Ptr;

  explicit OnePointStopCriterion()
      : is_true_(false) { }

  ~OnePointStopCriterion() { }

  /**
   * Init data for future test of current point.
   */
  virtual void init(const Point& point,
                    typename TFunction::Ptr function,
                    Set::Ptr set) = 0;
  virtual void init(const Point& point, typename TFunction::Ptr function) {
    init(point, function, NULL);
  }
  /**
   * Update data for future test of current point.
   */
  virtual void update(const Point& point) = 0;

  bool isTrue() const { return is_true_; }
  virtual typename OnePointStopCriterion<TFunction>::Ptr clone() const = 0;

  virtual string getDefaultAlias() const { return "OnePointStopCriterion"; }
  GET(typename TFunction::Ptr, function)
  GET(Set::Ptr, set)
 protected:
  SET(typename TFunction::Ptr, function)
  SET(Set::Ptr, set)
  typename TFunction::Ptr function_;
  typename Set::Ptr set_;
  SET(bool, is_true)
 private:
  /**
   * Current is_true result for current point
   * and stop criterion.
   */
  bool is_true_;
};
}
#endif  // LTR_OPTIMIZATION_STOP_CRITERIA_ONE_POINT_STOP_CRITERION_H_

