// Copyright 2012 Yandex

#ifndef LTR_OPTIMIZATION_SETS_BALL_SET_H_
#define LTR_OPTIMIZATION_SETS_BALL_SET_H_

#include <string>
#include "ltr/utility/macros.h"
#include "ltr/optimization/sets/set.h"

namespace optimization {
/**
 * \class\brief Implements ball of a given radius with
 *
 * Center in a given point
 * \see Set
 */
class BallSet : public Set {
 public:
  typedef ltr::utility::shared_ptr<BallSet> Ptr;

  BallSet(double radius, const Point& center);
  /**
   * checks, whether given point is inside this set
   */
  virtual bool isInside(const Point& point) const;
  /**
   * projects given point on this set
   */
  virtual void computeProjection(const Point& point, Point* projection) const;
   /**
   * give a cubic boundaries which contain this set
   */
  virtual void getBoundaries(Point* top, Point* bottom) const;
  /**
   * get arbitrary point in given set
   */
  virtual void sampleRandomPointInside(Point* point) const;

  GET_SET(double, radius);
  GET_SET(Point, center);

  virtual string getDefaultAlias() const { return "BallSet"; }
 private:
  /**
   * radius of ball set
   */
  double radius_;
  /**
   * center of ball set
   */
  Point center_;
};
}

#endif  // LTR_OPTIMIZATION_SETS_BALL_SET_H_
