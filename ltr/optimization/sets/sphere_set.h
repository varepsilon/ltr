// Copyright 2013 Yandex

#ifndef LTR_OPTIMIZATION_SETS_SPHERE_SET_H_
#define LTR_OPTIMIZATION_SETS_SPHERE_SET_H_

#include <string>
#include "ltr/utility/macros.h"
#include "ltr/utility/numerical.h"
#include "ltr/optimization/sets/set.h"

namespace optimization {
/**
 * \brief Implements sphere of a given radius with center in a given point.
 *
 * \see Set
 */
class SphereSet : public Set {
 public:
  typedef ltr::utility::shared_ptr<SphereSet> Ptr;

  SphereSet(double radius, const Point& center);
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

  virtual string getDefaultAlias() const { return "SphereSet"; }
 private:
  /**
   * Radius of sphere set.
   */
  double radius_;
  /**
   * Center of sphere set.
   */
  Point center_;
};
}

#endif  // LTR_OPTIMIZATION_SETS_SPHERE_SET_H_
