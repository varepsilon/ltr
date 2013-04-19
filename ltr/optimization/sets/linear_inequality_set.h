// Copyright 2013 Yandex

#ifndef LTR_OPTIMIZATION_SETS_LINEAR_INEQUALITY_SET_H_
#define LTR_OPTIMIZATION_SETS_LINEAR_INEQUALITY_SET_H_

#include <string>
#include "ltr/utility/macros.h"
#include "ltr/interfaces/parameterized.h"
#include "ltr/optimization/sets/set.h"
#include "ltr/optimization/functions/function.h"

namespace optimization {
/**
 * \brief Implements semi-space define solutions of inequality.
 *
 * \f$ ( \overrightarrow{x}, \overrightarrow{a}) > 0 \f$
 * \see Set
 */
class LinearInequalitySet : public Set {
 public:
  typedef ltr::utility::shared_ptr<LinearInequalitySet> Ptr;

  LinearInequalitySet(const Vector& positive_normal,
                      double shift_);
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
  virtual void sampleRandomPointInside(Point* random_point) const;

  GET_SET(Vector, positive_normal);
  GET_SET(double, shift);
  virtual string getDefaultAlias() const { return "LinearInequalitySet"; }
 private:
  /**
   * Positive normal vector to border hyperplane.
   */
  Vector positive_normal_;
  /**
   * Shift above that hyperplane.
   */
  double shift_;
};
}

#endif  // LTR_OPTIMIZATION_SETS_LINEAR_INEQUALITY_SET_H_
