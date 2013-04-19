// Copyright 2012 Yandex

#ifndef LTR_OPTIMIZATION_SETS_POLYHEDRON_SET_H_
#define LTR_OPTIMIZATION_SETS_POLYHEDRON_SET_H_

#include <vector>
#include <string>
#include <limits>
#include "ltr/optimization/sets/set.h"
#include "ltr/utility/macros.h"
using std::vector;

namespace optimization {
/**
 * \brief Class for polyhedron sets in \f$\mathbb{R}^n\f$.
 *
 * Set is defined by set of equations
 * \f$A\overrightarrow{x} \le \overrightarrow{b}\f$
 * \see Set
 */
class PolyhedronSet : public Set {
 public:
  typedef ltr::utility::shared_ptr<PolyhedronSet> Ptr;

  explicit PolyhedronSet(const Matrix& a,
                         const Vector& b,
                         double precision = 0.01);
  ~PolyhedronSet() { }

  bool isInside(const Point& point) const;
  /**
   * Project point on the polyhedron via method of successive projections.
   */
  void computeProjection(const Point& point, Point* projection) const;
  /**
   * Get approximate boundaries of polyhedron set based on the assumption that
   * diam(polyhedron) is approximately 10.0 * fabs(b_i) / sum(fabs(A_i)).
   */
  void getBoundaries(Point* top, Point* bottom) const;
  void sampleRandomPointInside(Point* random_point) const;

  string getDefaultAlias() const {
    return "PolyhedronSet";
  }

  GET_SET(Matrix, A)
  GET_SET(Vector, b)
  GET_SET(double, precision)
 private:
  /**
   * Project current point on the furthest plane of polyhedron.
   */
  void projectOnFurthestPlane(const Point& point, Point* projection) const;
  /**
   * Is this point break limit number limit_id.
   *
   * \return True if yes.
   */
  bool isInequalitySatisfied(const Point& point, int limit_id) const;
  /**
   * Compute distance from current point to the plane with number plane_id
   * if margin < 0 then point is outside the set.
   */
  double computeMargin(const Point& point, int plane_id) const;

  Matrix A_;
  Vector b_;
  /**
   * Space where point is inside in the polyhedron near it.
   */
  double precision_;
};
}

#endif  // LTR_OPTIMIZATION_SETS_POLYHEDRON_SET_H_
