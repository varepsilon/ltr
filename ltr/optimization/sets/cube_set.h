// Copyright 2012 Yandex

#ifndef LTR_OPTIMIZATION_SETS_CUBE_SET_H_
#define LTR_OPTIMIZATION_SETS_CUBE_SET_H_

#include <string>
#include "ltr/utility/macros.h"
#include "ltr/optimization/sets/set.h"

namespace optimization {
/**
 * \brief Implements cube of a given size with center in the
 * \f$(0, \dots, 0)\f$. <br>
 * Every point of the set \f$\in [-size;size]\times \dots \times [-size;size]\f$.
 *
 * \see Set
 */
class CubeSet : public Set {
  ALLOW_SHARED_PTR_ONLY_CREATION(CubeSet)
 public:
  CubeSet(double size, int dimension);
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

  GET_SET(double, size);

  virtual string getDefaultAlias() const { return "CubeSet"; }
 private:
  /**
   * Size of the cube set.
   */
  double size_;
};
}

#endif  // LTR_OPTIMIZATION_SETS_CUBE_SET_H_
