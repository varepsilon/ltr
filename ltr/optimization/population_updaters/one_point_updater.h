// Copyright 2013 Yandex

#ifndef LTR_OPTIMIZATION_POPULATION_UPDATERS_ONE_POINT_UPDATER_H_
#define LTR_OPTIMIZATION_POPULATION_UPDATERS_ONE_POINT_UPDATER_H_

#include <string>
#include "contrib/include_Eigen.h"
#include "ltr/utility/macros.h"
#include "ltr/interfaces/aliaser.h"
#include "ltr/utility/boost/shared_ptr.h"
#include "ltr/optimization/sets/set.h"

namespace optimization {
typedef Eigen::VectorXd Point;
typedef Eigen::VectorXd Vector;
/**
 * \brief Abtract class for update one point from population
 * independently on other points.
 *
 * \tparam Is a type of function.
 *
 * \see PopulationUpdater, PerPointUpdater
 */
template<class TFunction>
class OnePointUpdater : public ltr::Aliaser {
 public:
  typedef ltr::utility::shared_ptr< OnePointUpdater<TFunction> > Ptr;

  explicit OnePointUpdater() { }
  virtual ~OnePointUpdater() { }

  /**
   * Init data for future updates of current point.
   */
  virtual void init(const Point& point,
                    typename TFunction::Ptr function,
                    Set::Ptr set) = 0;
  /**
   * \brief Update point via one of iteration methods.
   */
  virtual void update(const Point& point, Point* updated_point) = 0;
  /**
   * Create deep copy of current one point updater.
   */
  virtual typename OnePointUpdater<TFunction>::Ptr clone() const = 0;

  virtual string getDefaultAlias() const {
    return "OnePointUpdater";
  }
  GET(typename TFunction::Ptr, function)
  GET(Set::Ptr, set)
 protected:
  SET(typename TFunction::Ptr, function)
  SET(Set::Ptr, set)
  typename TFunction::Ptr function_;
  Set::Ptr set_;
};
}
#endif  // LTR_OPTIMIZATION_POPULATION_UPDATERS_ONE_POINT_UPDATER_H_
