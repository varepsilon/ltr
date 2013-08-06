// Copyright 2013 Yandex

#include <gtest/gtest.h>

#include "ltr/utility/numerical.h"
#include "ltr/optimization/sets/ball_set.h"
#include "ltr/optimization/sets/linear_inequality_set.h"
#include "ltr/optimization/sets/sphere_set.h"
#include "ltr/optimization/sets/polyhedron_set.h"
#include "ltr/optimization/sets/cube_set.h"

using ltr::utility::DoubleEqual;

using optimization::BallSet;
using optimization::Point;
using optimization::Vector;
using optimization::Matrix;
using optimization::LinearInequalitySet;
using optimization::SphereSet;
using optimization::PolyhedronSet;
using optimization::CubeSet;

TEST(SetTest, BallSetInsideTest) {
  Point center(2);
  center[0] = 0.0;
  center[1] = 0.0;

  BallSet set(10.0, center);

  Point point(2);
  point[0] = 3;
  point[1] = 3;
  EXPECT_TRUE(set.isInside(point));

  point[0] = 0;
  point[1] = -9.99;
  EXPECT_TRUE(set.isInside(point));

  point[0] = 7.6;
  point[1] = 7.7;
  EXPECT_FALSE(set.isInside(point));

  Point wrong_dimension_point(1);
  EXPECT_ANY_THROW(set.isInside(wrong_dimension_point));
}
TEST(SetTest, BallSetProjectTest) {
  Point center(2);
  center[0] = 0.0;
  center[1] = 0.0;

  BallSet set(10.0, center);

  Point point(2);
  point[0] = 30.0;
  point[1] = 40.0;

  Point projected_point;
  set.computeProjection(point, &projected_point);

  EXPECT_TRUE(DoubleEqual(projected_point[0], 6.0));
  EXPECT_TRUE(DoubleEqual(projected_point[1], 8.0));
  EXPECT_TRUE(set.isInside(projected_point));
}
TEST(SetTest, BallSetBoundariesTest) {
  Point center(4);
  center << 0.0, 0.0, 0.0, 0.0;

  BallSet set(6.0, center);

  Point top(4);
  Point bottom(4);

  set.getBoundaries(&top, &bottom);

  EXPECT_TRUE(DoubleEqual(top[0] + bottom[1], 0));

  Point wrong_dimension_point(1);
  EXPECT_ANY_THROW(set.getBoundaries(&top, &wrong_dimension_point));
}
TEST(SetTest, BallSetRandomPointTest) {
  Point center(10);
  center << 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0;

  BallSet set(5.0, center);

  Point random_point(10);
  EXPECT_NO_THROW(set.sampleRandomPointInside(&random_point));
  EXPECT_TRUE(set.isInside(random_point));
}
TEST(SetTest, NonZeroBallSetInsideTest) {
  Point center(10);
  center << 1.0, 2.0, 0.0, -1.0, 3.0, -9.0, 10.0, 12.0, 6.0, 3.0;

  BallSet set(5.0, center);

  Point random_point(10);
  EXPECT_NO_THROW(set.sampleRandomPointInside(&random_point));
  EXPECT_TRUE(set.isInside(random_point));
}
TEST(SetTest, NonZeroBallSetProjectionTest) {
  Point center(3);
  center << 2.0, 2.0, 2.0;

  BallSet set(sqrt(3.0), center);

  Point projected_point(3);
  projected_point.setZero();

  Point projection;

  set.computeProjection(projected_point, &projection);
  EXPECT_DOUBLE_EQ(projection(0), 1.0);
}

TEST(SetTest, LinearInequalitySetInsideTest) {
  Vector normal(3);
  normal[0] = 2.0;
  normal[1] = 2.0;
  normal[2] = 0.0;
  LinearInequalitySet set(normal, 0.0);

  Point point(3);
  point[0] = 3;
  point[1] = 3;
  point[2] = -10;
  EXPECT_TRUE(set.isInside(point));

  point[0] = 0;
  point[1] = -10.0;
  EXPECT_FALSE(set.isInside(point));

  Point wrong_dimension_point(1);
  EXPECT_ANY_THROW(set.isInside(wrong_dimension_point));
}
TEST(SetTest, LinearInequalitySetProjectTest) {
  Vector normal(3);
  normal[0] = 2;
  normal[1] = 2;
  normal[2] = 0;

  LinearInequalitySet set(normal, 0.0);

  Point point(3);
  point[0] = +5.0;
  point[1] = +5.0;
  point[2] = 4.0;

  Point projection;
  set.computeProjection(point, &projection);
  EXPECT_DOUBLE_EQ(projection[0], 5.0);
  EXPECT_DOUBLE_EQ(projection[1], 5.0);
  EXPECT_TRUE(set.isInside(point));
}
TEST(SetTest, NonZeroLinearInequalitySetProjectTest) {
  Vector normal(2);
  normal << 1.0, 1.0;
  double shift = -1.0;

  LinearInequalitySet set(normal, shift);

  Point point(2);
  point << 0.0, 0.0;

  Point projection;
  set.computeProjection(point, &projection);

  EXPECT_DOUBLE_EQ(projection[0], 0.5);
  EXPECT_DOUBLE_EQ(projection[1], 0.5);
}
TEST(SetTest, LinearInequalitySetBoundariesTest) {
  Vector normal(3);
  normal[0] = 2;
  normal[1] = 2;
  normal[2] = 0;

  LinearInequalitySet set(normal, 0.0);

  Point top(3);
  Point bottom(3);

  EXPECT_NO_THROW(set.getBoundaries(&top, &bottom));

  Point wrong_dimension_point(1);
  EXPECT_ANY_THROW(set.getBoundaries(&top, &wrong_dimension_point));
}
TEST(SetTest, LinearInequalitySetRandomPointTest) {
  Vector normal(3);
  normal[0] = 2;
  normal[1] = 2;
  normal[2] = 0;

  LinearInequalitySet set(normal, 0.0);

  Point random_point(3);
  EXPECT_NO_THROW(set.sampleRandomPointInside(&random_point));
  EXPECT_TRUE(set.isInside(random_point));
}

TEST(SetTest, SphereSetInsideTest) {
  Point center(2);
  center << 0.0, 0.0;
  SphereSet set(10.0, center);

  Point point(2);
  point[0] = 3;
  point[1] = 3;
  EXPECT_FALSE(set.isInside(point));

  point[0] = 0;
  point[1] = -10.0;
  EXPECT_TRUE(set.isInside(point));

  point[0] = 6.1;
  point[1] = 8.1;
  EXPECT_FALSE(set.isInside(point));

  Point wrong_dimension_point(1);
  EXPECT_ANY_THROW(set.isInside(wrong_dimension_point));
}
TEST(SetTest, SphereSetProjectTest) {
  Point center(2);
  center << 0.0, 0.0;
  SphereSet set(10.0, center);

  Point point(2);
  point[0] = 30.0;
  point[1] = 40.0;

  Point projection;
  set.computeProjection(point, &projection);
  EXPECT_TRUE(DoubleEqual(projection[0], 6.0));
  EXPECT_TRUE(DoubleEqual(projection[1], 8.0));
  EXPECT_TRUE(set.isInside(projection));
}
TEST(SetTest, SphereSetBoundariesTest) {
  Point center(4);
  center << 0.0, 0.0, 0.0, 0.0;
  SphereSet set(6.0, center);

  Point top(4);
  Point bottom(4);

  set.getBoundaries(&top, &bottom);

  EXPECT_TRUE(DoubleEqual(top[0] + bottom[1], 0));

  Point wrong_dimension_point(1);
  EXPECT_ANY_THROW(set.getBoundaries(&top, &wrong_dimension_point));
}
TEST(SetTest, SphereSetRandomPointTest) {
  Point center(2);
  center << 0.0, 0.0;
  SphereSet set(5.0, center);

  Point random_point;
  EXPECT_NO_THROW(set.sampleRandomPointInside(&random_point));
  EXPECT_TRUE(set.isInside(random_point));
}
TEST(SetTest, NonZeroSphereSetInsideTest) {
  Point center(10);
  center << 1.0, 2.0, 0.0, -1.0, 3.0, -9.0, 10.0, 12.0, 6.0, 3.0;

  SphereSet set(5.0, center);

  Point random_point(10);
  EXPECT_NO_THROW(set.sampleRandomPointInside(&random_point));
  EXPECT_TRUE(set.isInside(random_point));
}
TEST(SetTest, NonZeroSphereSetProjectionTest) {
  Point center(3);
  center << 2.0, 2.0, 2.0;

  SphereSet set(sqrt(3.0), center);

  Point projected_point(3);
  projected_point.setZero();

  Point projection;

  set.computeProjection(projected_point, &projection);
  EXPECT_DOUBLE_EQ(projection(0), 1.0);

  projected_point.setConstant(sqrt(3.0) + 0.01);
  set.computeProjection(projected_point, &projection);
  EXPECT_DOUBLE_EQ(projection(0), 1.0);

  projected_point = set.center();
  EXPECT_NO_THROW(set.computeProjection(projected_point, &projection));
}

TEST(SetTest, PolyhedronSetInsideTest) {
  Matrix a(3, 2);
  a << -1 ,  0,
        0 , -1,
        1 ,  1;

  Vector b(3);
  b << 0, 0, 1;

  PolyhedronSet set(a, b);
  Point point(2);
  point << 1, 1;
  EXPECT_FALSE(set.isInside(point));
  point << -1, -1;
  EXPECT_FALSE(set.isInside(point));
  point << 0.49, 0.49;
  EXPECT_TRUE(set.isInside(point));
}
TEST(SetTest, PolyhedronSetProjectionTest) {
  Matrix a(3, 2);
  a << -1 ,  0,
        0 , -1,
        1 ,  1;

  Vector b(3);
  b << 0, 0, 1;

  PolyhedronSet set(a, b);

  Point point(2);
  Point projection(2);
  Point true_projection(2);

  point << 1, 1;
  true_projection << 0.5, 0.5;

  set.computeProjection(point, &projection);
  EXPECT_TRUE(projection.isApprox(true_projection, 0.01));

  point << -1, -1;
  true_projection << 0, 0;

  set.computeProjection(point, &projection);
  EXPECT_TRUE(projection.isApprox(true_projection, 0.01));

  point << 100, -1;
  true_projection << 1, 0;

  set.computeProjection(point, &projection);
  EXPECT_TRUE(projection.isApprox(true_projection, 0.01));
}
TEST(SetTest, PolyhedronSetProjectionBigTest) {
  Matrix a(25, 50);
  a.setRandom();
  Vector b(25);
  b.setRandom();

  PolyhedronSet set(a, b);
  Point point(50);
  Point projection;

  for (int i = 0; i < 10; ++i) {
    point.setRandom();
    if (!set.isInside(point)) {
      set.computeProjection(point, &projection);
      EXPECT_TRUE(set.isInside(projection));
    }
  }
}
TEST(SetTest, PolyhedronSetSampleRandomPointTest) {
  Matrix a(3, 2);
  a << -1 ,  0,
        0 , -1,
        1 ,  1;

  Vector b(3);
  b << 0, 0, 1;

  PolyhedronSet set(a, b);
  for (int test_point_id = 0; test_point_id < 100; ++test_point_id) {
    Point point;
    set.sampleRandomPointInside(&point);
    EXPECT_TRUE(set.isInside(point));
  }
}

TEST(SetTest, CubeSetInsideTest) {
  CubeSet::Ptr set(new CubeSet(5.0, 2));

  Point point(2);
  point[0] = 3;
  point[1] = 3;
  EXPECT_TRUE(set->isInside(point));

  point[0] = 4.99;
  point[1] = -4.99;
  EXPECT_TRUE(set->isInside(point));

  point[0] = 5.01;
  point[1] = 0;
  EXPECT_FALSE(set->isInside(point));

  Point wrong_dimension_point(1);
  EXPECT_ANY_THROW(set->isInside(wrong_dimension_point));
}
TEST(SetTest, CubeSetProjectionTest) {
  CubeSet::Ptr set(new CubeSet(10.0, 2));

  Point point(2);
  point[0] = 30.0;
  point[1] = -40.0;

  Point projected_point;
  set->computeProjection(point, &projected_point);

  Point correct_projection(2);
  correct_projection << 10.0, -10.0;

  EXPECT_TRUE(correct_projection.isApprox(projected_point, 0.01));
  EXPECT_TRUE(set->isInside(projected_point));
}
TEST(SetTest, CubeSetBoundariesTest) {
  CubeSet::Ptr set(new CubeSet(10.0, 4));

  Point top(2);
  Point bottom(1);

  Point correct_top(4);
  Point correct_bottom(4);
  correct_top << 10.0, 10.0, 10.0, 10.0;
  correct_bottom << -10.0, -10.0, -10.0, -10.0;

  EXPECT_NO_THROW(set->getBoundaries(&top, &bottom));
  EXPECT_TRUE(correct_top.isApprox(top, 0.01));
  EXPECT_TRUE(correct_bottom.isApprox(bottom, 0.01));
}
TEST(SetTest, CubeSetRandomPointTest) {
  CubeSet::Ptr set(new CubeSet(2.0, 3));
  Point random_point(1);
  EXPECT_NO_THROW(set->sampleRandomPointInside(&random_point));
  EXPECT_TRUE(set->isInside(random_point));
}
