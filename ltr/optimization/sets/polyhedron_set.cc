// Copyright 2012 Yandex

#include "ltr/utility/numerical.h"
#include "ltr/optimization/sets/polyhedron_set.h"

namespace optimization {
PolyhedronSet::PolyhedronSet(const Matrix& A,
                             const Vector& b,
                             double precision)
    : Set(A.cols()), precision_(precision) {
  CHECK(A.rows() == b.rows());
  CHECK(precision > 0.0);
  A_ = A;
  b_ = b;
}

bool PolyhedronSet::isInside(const Point& point) const {
  Point margins = A_ * point - b_;
  for (int row_id = 0; row_id < b_.rows(); ++row_id) {
    if (margins(row_id) > precision_) {
      return false;
    }
  }
  return true;
}

double PolyhedronSet::computeMargin(const Point& point, int plane_id) const {
  double norm_a = A_.row(plane_id).norm();
  Vector a = A_.row(plane_id) / norm_a;
  double b = b_(plane_id) / norm_a;

  return b - point.dot(a);
}

bool PolyhedronSet::isInequalitySatisfied(const Point& point,
                                          int plane_id) const {
  /**
   * if margin  < 0 then point outside
   */
  double margin = b_(plane_id) - point.dot(A_.row(plane_id));
  return ltr::utility::DoubleLess(margin, 0);
}

void PolyhedronSet::projectOnFurthestPlane(const Point& point,
                                           Point* projection) const {
  int furthest_plane_id = 0;
  double current_margin = 0.0;
  for (int plane_id = 0; plane_id < A_.rows(); ++plane_id) {
    if (isInequalitySatisfied(point, plane_id)) {
      double margin = computeMargin(point, plane_id);
      if (margin < current_margin) {
        current_margin = margin;
        furthest_plane_id = plane_id;
      }
    }
  }
  Point a = A_.row(furthest_plane_id).transpose() /
            A_.row(furthest_plane_id).norm();
  double b = b_(furthest_plane_id) / A_.row(furthest_plane_id).norm();

  *projection = point - a * (a.dot(point) - b);
}

void PolyhedronSet::computeProjection(const Point& point,
                                      Point* projection) const {
  Point current_projection = point;
  while (!isInside(current_projection)) {
    projectOnFurthestPlane(current_projection, &current_projection);
  }
  *projection = current_projection;
}

void PolyhedronSet::getBoundaries(Point* top, Point* bottom) const {
  double top_boundary = 0;
  double current_top_boundary;
  for (int limit = 0; limit < A_.rows(); ++limit) {
    current_top_boundary = 10.0 * fabs(b_(limit)) / A_.row(limit).lpNorm<1>();
    if (current_top_boundary > top_boundary) {
      top_boundary = current_top_boundary;
    }
  }
  top->setConstant(top_boundary);
  bottom->setConstant(-top_boundary);
}

void PolyhedronSet::sampleRandomPointInside(Point* point) const {
  Point random_point_in_boundaries;
  getRandomPoint(&random_point_in_boundaries);
  computeProjection(random_point_in_boundaries, point);
}
}
