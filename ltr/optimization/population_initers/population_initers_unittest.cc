// Copyright 2013 Yandex

#include <gtest/gtest.h>
#include "ltr/optimization/population_initers/population.h"
#include "ltr/optimization/population_initers/naive_initer.h"
#include "ltr/optimization/sets/ball_set.h"

using optimization::NaiveIniter;
using optimization::Population;
using optimization::Point;
using optimization::BallSet;

TEST(IteratorTest, EqNeqPostfixOperatorTest) {
  Population population;
  Point a(2);
  a << 1, 1;
  Point b(2);
  b << 2, 2;
  Point c(2);
  c << 3, 3;

  population.addPoint(a);
  population.addPoint(b);
  population.addPoint(c);
  population.removePoint(1);
  Population::Iterator iterator(population.begin());
  EXPECT_EQ(iterator.point_id(), 0);
  EXPECT_FALSE(iterator == population.end());
  EXPECT_TRUE(iterator != population.end());
  EXPECT_TRUE(iterator == population.begin());
  EXPECT_FALSE(iterator != population.begin());

  iterator++;
  EXPECT_EQ(iterator.point_id(), 2);
  EXPECT_FALSE(iterator == population.end());
  EXPECT_FALSE(iterator == population.end());
  EXPECT_FALSE(iterator == population.begin());
  EXPECT_FALSE(iterator == population.begin());
  EXPECT_TRUE(iterator != population.end());
  EXPECT_TRUE(iterator != population.end());
  EXPECT_TRUE(iterator != population.begin());
  EXPECT_TRUE(iterator != population.begin());

  iterator++;
  EXPECT_TRUE(iterator == population.end());
  EXPECT_FALSE(iterator != population.end());
  EXPECT_TRUE(iterator != population.begin());
  EXPECT_FALSE(iterator == population.begin());

  iterator--;
  EXPECT_EQ(iterator.point_id(), 2);
}
TEST(IteratorTest, PrefixOperatorTest) {
  Population population;
  Point a(2);
  a << 1, 1;
  Point b(2);
  b << 2, 2;
  Point c(2);
  c << 3, 3;

  population.addPoint(a);
  population.addPoint(b);
  population.addPoint(c);
  population.removePoint(1);
  Population::Iterator iterator(population.begin());

  ++iterator;
  EXPECT_EQ(iterator.point_id(), 2);
  EXPECT_EQ(iterator.point()(1), 3);
  EXPECT_FALSE(iterator == population.end());

  ++iterator;
  EXPECT_TRUE(iterator == population.end());

  --iterator;
  EXPECT_EQ(iterator.point_id(), 2);
}
TEST(IteratorTest, CopyConstructorTest) {
  Population population;
  Point a(2);
  a << 1, 1;
  Point b(2);
  b << 2, 2;
  Point c(2);
  c << 3, 3;

  population.addPoint(a);
  population.addPoint(b);
  population.addPoint(c);
  population.removePoint(1);
  Population::Iterator iterator(population.begin());
  ++iterator;
  iterator = population.begin();
  EXPECT_EQ(iterator.point_id(), 0);
  EXPECT_EQ(iterator.point()(1), 1);
  EXPECT_FALSE(iterator == population.end());
  EXPECT_TRUE(iterator == population.begin());

  ++iterator;
  EXPECT_TRUE(iterator != population.end());
  iterator = population.end();
  EXPECT_TRUE(iterator == population.end());

  --iterator;
  EXPECT_EQ(iterator.point_id(), 2);
}

TEST(PopulationTest, PopulationAddRemoveGetPointTest) {
  Population population;
  Point a(2);
  a << 1, 1;
  Point b(2);
  b << 2, 2;
  Point c(2);
  c << 3, 3;

  population.addPoint(a);
  population.addPoint(b);
  population.addPoint(c);
  population.addPoint(b);
  population.removePoint(1);
  Point point;
  population.getPoint(0, &point);
  EXPECT_EQ(point(1), 1);
  population.getPoint(3, &point);
  EXPECT_EQ(point(1), 2);
}

TEST(PopulationTest, PopulationSizeTest) {
  Population population;
  Point a(2);
  a << 1, 1;
  Point b(2);
  b << 2, 2;
  Point c(2);
  c << 3, 3;

  population.addPoint(a);
  population.addPoint(b);
  population.addPoint(c);
  population.removePoint(1);
  EXPECT_EQ(population.size(), 2);
  population.addPoint(a);
  EXPECT_EQ(population.size(), 3);
}

TEST(PopulationTest, PopulationIniterTest) {
  Point center(2);
  center.setZero();
  BallSet set(2.0, center);
  NaiveIniter population_initer;

  Population population;
  population_initer.populationInit(10, set, &population);

  population.removePoint(1);
  EXPECT_EQ(9, population.size());
  Population::Iterator it = population.begin();
  ++it;
  EXPECT_EQ(it.point_id(), 2);
  for (Population::Iterator it = population.begin();
       it != population.end();
       ++it) {
    EXPECT_TRUE(set.isInside(it.point()));
  }
}
