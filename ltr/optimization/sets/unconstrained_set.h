// Copyright 2013 Yandex

#ifndef LTR_OPTIMIZATION_SETS_UNCONSTRAINED_SET_H_
#define LTR_OPTIMIZATION_SETS_UNCONSTRAINED_SET_H_

#include <string>
#include "ltr/optimization/sets/set.h"

namespace optimization {
/**
 * \brief Interface class for unconstrained set in \f$\mathbb{R}^n\f$.
 *
 * \see Set
 */
class UnconstrainedSet : public Set {
 public:
  typedef ltr::utility::shared_ptr<UnconstrainedSet> Ptr;

  explicit UnconstrainedSet(int dimension);
  virtual ~UnconstrainedSet();
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
  virtual string getDefaultAlias() const { return "UnconstrainedSet"; }
};
}

#endif  // LTR_OPTIMIZATION_SETS_UNCONSTRAINED_SET_H_
