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
#include "ltr/measures/binary_classification_margin.h"
#include "ltr/measures/binary_classification_accuracy.h"

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
    object1 << 1;
    object1.set_actual_label(1.0);
    object1.set_predicted_label(1.0);

    object2 << 2;
    object2.set_actual_label(1.5);
    object2.set_predicted_label(2.3);

    object3 << 3;
    object3.set_actual_label(0.5);
    object3.set_predicted_label(-0.5);

    object4 << 4;
    object4.set_actual_label(3.1);
    object4.set_predicted_label(3.1);

    object5 << 5;
    object5.set_actual_label(0.76);
    object5.set_predicted_label(2.13);
  }

  virtual void TearDown() {
  }

  protected:
    Object object1;
    Object object2;
    Object object3;
    Object object4;
    Object object5;
};

TEST_F(PointwiseMeasuresTest, TestingAbsError) {
  AbsError ae;
  EXPECT_TRUE(DoubleEqual(ae(object1), 0));
  EXPECT_TRUE(DoubleEqual(ae(object2), 0.8));
  EXPECT_TRUE(DoubleEqual(ae(object3), 1));
  EXPECT_TRUE(DoubleEqual(ae(object4), 0));
  EXPECT_TRUE(DoubleEqual(ae(object5), 1.37));
}

TEST_F(PointwiseMeasuresTest, TestingAccuracy) {
  Accuracy<Object> ac;
  EXPECT_EQ(1, ac(object1));
  EXPECT_EQ(0, ac(object2));
  EXPECT_EQ(0, ac(object3));
  EXPECT_EQ(1, ac(object4));
  EXPECT_EQ(0, ac(object5));
}

TEST_F(PointwiseMeasuresTest, TestingSquaredError) {
  SquaredError se;
  EXPECT_TRUE(DoubleEqual(se(object1), 0));
  EXPECT_TRUE(DoubleEqual(se(object2), 0.64));
  EXPECT_TRUE(DoubleEqual(se(object3), 1));
  EXPECT_TRUE(DoubleEqual(se(object4), 0));
  EXPECT_TRUE(DoubleEqual(se(object5), 1.8769));
}

TEST_F(PointwiseMeasuresTest, TestingTruePoint) {
  TruePoint tp;
  EXPECT_TRUE(DoubleEqual(tp(object1), 1.0));
  EXPECT_TRUE(DoubleEqual(tp(object2), 0.555555555555556));
  EXPECT_TRUE(DoubleEqual(tp(object3), 0.5));
  EXPECT_TRUE(DoubleEqual(tp(object4), 1.0));
  EXPECT_TRUE(DoubleEqual(tp(object5), 0.421940928270042));
}
