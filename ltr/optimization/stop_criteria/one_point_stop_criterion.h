// Copyright 2012 Yandex

#ifndef LTR_OPTIMIZATION_STOP_CRITERIA_ONE_POINT_STOP_CRITERION_H_
#define LTR_OPTIMIZATION_STOP_CRITERIA_ONE_POINT_STOP_CRITERION_H_

#include <string>
#include "Eigen/Core"
#include "ltr/utility/macros.h"
#include "ltr/interfaces/aliaser.h"
#include "ltr/utility/boost/shared_ptr.h"

namespace optimization {
typedef Eigen::VectorXd Point;
/**
 * \brief Abtract class for test one point from population
 * independently on other points.
 *
 * \see StopCriterion, PerPointStopCriterion
 */
template<class TFunction> class OnePointStopCriterion : public ltr::Aliaser {
 public:
  typedef ltr::utility::shared_ptr< OnePointStopCriterion<TFunction> > Ptr;

  explicit OnePointStopCriterion(typename TFunction::Ptr function)
      : function_(function),
        is_true_(false) { }

  ~OnePointStopCriterion() { }

  /**
   * Init data for future test of current point.
   */
  virtual void init(const Point& point) = 0;
  /**
   * Update data for future test of current point.
   */
  virtual void update(const Point& point) = 0;

  GET_SET(typename TFunction::Ptr, function);
  bool isTrue() const { return is_true_; }
  virtual typename OnePointStopCriterion<TFunction>::Ptr clone() const = 0;

  virtual string getDefaultAlias() const { return "OnePointStopCriterion"; }
 protected:
  typename TFunction::Ptr function_;
  SET(bool, is_true);
 private:
  /**
   * Current is_true result for current point
   * and stop criterion.
   */
  bool is_true_;
};
}
#endif  // LTR_OPTIMIZATION_STOP_CRITERIA_ONE_POINT_STOP_CRITERION_H_

