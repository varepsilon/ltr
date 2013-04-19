// Copyright 2012 Yandex

#ifndef LTR_OPTIMIZATION_SETS_BALL_SET_H_
#define LTR_OPTIMIZATION_SETS_BALL_SET_H_

#include <string>
#include "ltr/utility/macros.h"
#include "ltr/optimization/sets/set.h"

namespace optimization {
/**
 * \brief Implements ball of a given radius with center in a given point.
 *
 * \see Set
 */
class BallSet : public Set {
 public:
  typedef ltr::utility::shared_ptr<BallSet> Ptr;

  BallSet(double radius, const Point& center);
  /**
   * Checks, whether given point is inside this set.
   */
  virtual bool isInside(const Point& point) const;
  /**
   * Projects given point on this set.
   */
  virtual void computeProjection(const Point& point, Point* projection) const;
   /**
    * Give a cubic boundaries which contain this set.
    */
  virtual void getBoundaries(Point* top, Point* bottom) const;
  /**
   * Get arbitrary point in given set.
   */
  virtual void sampleRandomPointInside(Point* point) const;

  GET_SET(double, radius);
  GET_SET(Point, center);

  virtual string getDefaultAlias() const { return "BallSet"; }
 private:
  /**
   * Radius of ball set.
   */
  double radius_;
  /**
   * Center of ball set.
   */
  Point center_;
};
}

#endif  // LTR_OPTIMIZATION_SETS_BALL_SET_H_
