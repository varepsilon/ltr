// Copyright 2011 Yandex School Practice

#include <gtest/gtest.h>

#include <vector>

#include "ltr/utility/numerical.h"
#include "ltr/data/object.h"
#include "ltr/parameters_container/parameters_container.h"
#include "ltr/measures/abs_error.h"
#include "ltr/measures/accuracy.h"
#include "ltr/measures/squared_error.h"
#include "ltr/measures/true_point.h"

using ltr::Object;
using ltr::utility::DoubleEqual;
using ltr::ParametersContainer;
using ltr::AbsError;
using ltr::Accuracy;
using ltr::SquaredError;
using ltr::TruePoint;

class PointwiseMeasuresTest : public ::testing::Test {
  protected:
  virtual void SetUp() {
    o1 << 1;
    o1.setActualLabel(1.0);
    o1.setPredictedLabel(1.0);

    o2 << 2;
    o2.setActualLabel(1.5);
    o2.setPredictedLabel(2.3);

    o3 << 3;
    o3.setActualLabel(0.5);
    o3.setPredictedLabel(-0.5);

    o4 << 4;
    o4.setActualLabel(3.1);
    o4.setPredictedLabel(3.1);

    o5 << 5;
    o5.setActualLabel(0.76);
    o5.setPredictedLabel(2.13);
  }

  virtual void TearDown() {
  }

  protected:
    Object o1;
    Object o2;
    Object o3;
    Object o4;
    Object o5;
};

TEST_F(PointwiseMeasuresTest, TestingAbsError) {
  AbsError ae;
  EXPECT_TRUE(DoubleEqual(ae(o1), 0));
  EXPECT_TRUE(DoubleEqual(ae(o2), 0.8));
  EXPECT_TRUE(DoubleEqual(ae(o3), 1));
  EXPECT_TRUE(DoubleEqual(ae(o4), 0));
  EXPECT_TRUE(DoubleEqual(ae(o5), 1.37));
}

TEST_F(PointwiseMeasuresTest, TestingAccuracy) {
  Accuracy<Object> ac;
  EXPECT_EQ(1, ac(o1));
  EXPECT_EQ(0, ac(o2));
  EXPECT_EQ(0, ac(o3));
  EXPECT_EQ(1, ac(o4));
  EXPECT_EQ(0, ac(o5));
}

TEST_F(PointwiseMeasuresTest, TestingSquaredError) {
  SquaredError se;
  EXPECT_TRUE(DoubleEqual(se(o1), 0));
  EXPECT_TRUE(DoubleEqual(se(o2), 0.64));
  EXPECT_TRUE(DoubleEqual(se(o3), 1));
  EXPECT_TRUE(DoubleEqual(se(o4), 0));
  EXPECT_TRUE(DoubleEqual(se(o5), 1.8769));
}

TEST_F(PointwiseMeasuresTest, TestingTruePoint) {
  TruePoint tp;
  EXPECT_TRUE(DoubleEqual(tp(o1), 1.0));
  EXPECT_TRUE(DoubleEqual(tp(o2), 0.555555555555556));
  EXPECT_TRUE(DoubleEqual(tp(o3), 0.5));
  EXPECT_TRUE(DoubleEqual(tp(o4), 1.0));
  EXPECT_TRUE(DoubleEqual(tp(o5), 0.421940928270042));
}
