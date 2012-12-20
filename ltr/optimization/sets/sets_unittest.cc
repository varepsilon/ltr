// Copyright 2012 Yandex

#include <gtest/gtest.h>

#include "ltr/utility/numerical.h"

#include "ltr/optimization/sets/ball_set.h"
#include "ltr/optimization/sets/linear_inequality_set.h"
#include "ltr/optimization/sets/sphere_set.h"

using ltr::utility::DoubleEqual;

using optimization::BallSet;
using optimization::Point;
using optimization::Vector;
using optimization::LinearInequalitySet;
using optimization::SphereSet;

TEST(SetTest, BallSetInsideTest) {
  BallSet set(10.0, 2);

  Point point(2);
  point[0] = 3;
  point[1] = 3;
  EXPECT_TRUE(set.isInside(point));

  Point wrong_dimension_point(1);
  EXPECT_ANY_THROW(set.isInside(wrong_dimension_point));

  point[0] = 0;
  point[1] = -10.0;
  EXPECT_TRUE(set.isInside(point));

  point[0] = 3.6;
  point[1] = 3.7;
  EXPECT_FALSE(set.isInside(point));
}

TEST(SetTest, BallSetProjectTest) {
  BallSet set(10.0, 2);

  Point point(2);
  point[0] = 30.0;
  point[1] = 40.0;

  Point projected_point = set.project(point);
  EXPECT_TRUE(DoubleEqual(projected_point[0], 6.0));
  EXPECT_TRUE(DoubleEqual(projected_point[1], 8.0));
  EXPECT_TRUE(set.isInside(projected_point));
}

TEST(SetTest, BallSetBoundariesTest) {
  BallSet set(6.0, 4);

  Point top(4);
  Point bottom(4);

  set.getBoundaries(&top, &bottom);

  EXPECT_TRUE(DoubleEqual(top[0] + bottom[1], 0));

  Point wrong_dimension_point;
  EXPECT_ANY_THROW(set.getBoundaries(&top, &bottom));
}

TEST(SetTest, BallSetRandomPointTest) {
  BallSet set(5.0, 10);

  Point random_point;
  EXPECT_NO_THROW(random_point = set.getRandomPointInSet());
  EXPECT_TRUE(set.isInside(random_point));
}

TEST(SetTest, LinearInequalitySetInsideTest) {
  Vector normal(3);
  normal[0] = 2;
  normal[1] = 2;
  normal[2] = 0;

  Point zero_point(3);
  zero_point.setZero();

  LinearInequalitySet set(normal, zero_point, 3);

  Point point(3);
  point[0] = 3;
  point[1] = 3;
  point[2] = -10;
  EXPECT_TRUE(set.isInside(point));

  Point wrong_dimension_point(1);
  EXPECT_ANY_THROW(set.isInside(wrong_dimension_point));

  point[0] = 0;
  point[1] = -10.0;
  EXPECT_FALSE(set.isInside(point));
}

TEST(SetTest, LinearInequalitySetProjectTest) {
  Vector normal(3);
  normal[0] = 2;
  normal[1] = 2;
  normal[2] = 0;

  Point zero_point(3);
  zero_point.setZero();

  LinearInequalitySet set(normal, zero_point, 3);

  Point point(3);
  point[0] = -5.0;
  point[1] = -5.0;
  point[2] = 4.0;

  Point projected_point = set.project(point);
  EXPECT_TRUE(DoubleEqual(projected_point[0], 0.0));
  EXPECT_TRUE(DoubleEqual(projected_point[1], 0.0));
  EXPECT_TRUE(set.isInside(projected_point));
}

TEST(SetTest, LinearInequalitySetBoundariesTest) {
  Vector normal(3);
  normal[0] = 2;
  normal[1] = 2;
  normal[2] = 0;

  Point zero_point(3);
  zero_point.setZero();

  LinearInequalitySet set(normal, zero_point, 3);

  Point top(4);
  Point bottom(4);

  EXPECT_NO_THROW(set.getBoundaries(&top, &bottom));

  Point wrong_dimension_point;
  EXPECT_ANY_THROW(set.getBoundaries(&top, &bottom));
}

TEST(SetTest, LinearInequalitySetRandomPointTest) {
  Vector normal(3);
  normal[0] = 2;
  normal[1] = 2;
  normal[2] = 0;

  Point zero_point(3);
  zero_point.setZero();

  LinearInequalitySet set(normal, zero_point, 3);

  Point random_point;
  EXPECT_NO_THROW(random_point = set.getRandomPointInSet());
  EXPECT_TRUE(set.isInside(random_point));
}



TEST(SetTest, SphereSetInsideTest) {
  SphereSet set(10.0, 0.02, 2);

  Point point(2);
  point[0] = 3;
  point[1] = 3;
  EXPECT_FALSE(set.isInside(point));

  Point wrong_dimension_point(1);
  EXPECT_ANY_THROW(set.isInside(wrong_dimension_point));

  point[0] = 0;
  point[1] = -10.0;
  EXPECT_TRUE(set.isInside(point));

  point[0] = 6.1;
  point[1] = 8.1;
  EXPECT_FALSE(set.isInside(point));
}

TEST(SetTest, SphereSetProjectTest) {
  SphereSet set(10.0, 0.2, 2);

  Point point(2);
  point[0] = 30.0;
  point[1] = 40.0;

  Point projected_point = set.project(point);
  EXPECT_TRUE(DoubleEqual(projected_point[0], 6.0));
  EXPECT_TRUE(DoubleEqual(projected_point[1], 8.0));
  EXPECT_TRUE(set.isInside(projected_point));
}

TEST(SetTest, SphereSetBoundariesTest) {
  SphereSet set(6.0, 0.2, 4);

  Point top(4);
  Point bottom(4);

  set.getBoundaries(&top, &bottom);

  EXPECT_TRUE(DoubleEqual(top[0] + bottom[1], 0));

  Point wrong_dimension_point;
  EXPECT_ANY_THROW(set.getBoundaries(&top, &bottom));
}

TEST(SetTest, SphereSetRandomPointTest) {
  SphereSet set(5.0, 0.7, 10);

  Point random_point;
  EXPECT_NO_THROW(random_point = set.getRandomPointInSet());
  EXPECT_TRUE(set.isInside(random_point));
}
