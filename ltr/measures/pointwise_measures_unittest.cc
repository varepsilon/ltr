// Copyright 2011 Yandex School Practice

#include <gtest/gtest.h>

#include "ltr/utility/numerical.h"
#include "ltr/data/object.h"
#include "ltr/parameters_container/parameters_container.h"
#include "ltr/measures/abs_error.h"
#include "ltr/measures/accuracy.h"

using ltr::Object;
using ltr::utility::DoubleEqual;
using ltr::ParametersContainer;
using ltr::AbsError;
using ltr::Accuracy;

class PointMeasuresTest : public ::testing::Test {
  protected:
  virtual void SetUp() {
    o1 << 1;
    o1.setActualLabel(1.0);
    o1.setPredictedLabel(1.0);

    o2 << 2;
    o2.setActualLabel(1.5);
    o2.setPredictedLabel(2.3);

    o3 << 3;
    o3.setActualLabel(-0.5);
    o3.setPredictedLabel(0.5);

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

TEST_F(PointMeasuresTest, TestingAbsError) {
  AbsError ae;
  EXPECT_TRUE(DoubleEqual(ae(o1), 0));
  EXPECT_TRUE(DoubleEqual(ae(o2), 0.8));
  EXPECT_TRUE(DoubleEqual(ae(o3), 1));
  EXPECT_TRUE(DoubleEqual(ae(o4), 0));
  EXPECT_TRUE(DoubleEqual(ae(o5), 1.37));
}

TEST_F(PointMeasuresTest, TestingAccuracy) {
  Accuracy<Object> ac;
  EXPECT_EQ(1, ac(o1));
  EXPECT_EQ(0, ac(o2));
  EXPECT_EQ(0, ac(o3));
  EXPECT_EQ(1, ac(o4));
  EXPECT_EQ(0, ac(o5));
}
