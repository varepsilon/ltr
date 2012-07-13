// Copyright 2011 Yandex School Practice

#include <gtest/gtest.h>

#include "ltr/utility/numerical.h"
#include "ltr/data/object.h"
#include "ltr/data/object_pair.h"
#include "ltr/parameters_container/parameters_container.h"
#include "ltr/measures/accuracy.h"

using ltr::Object;
using ltr::ObjectPair;
using ltr::utility::DoubleEqual;
using ltr::ParametersContainer;
using ltr::Accuracy;


class PairwiseMeasuresTest : public ::testing::Test {
  protected:
  virtual void SetUp() {
    Object object1;
    object1 << 1;
    object1.set_actual_label(1);
    object1.set_predicted_label(2);
    Object object2;
    object2 << 2;
    object2.set_actual_label(2);
    object2.set_predicted_label(1);
    object_pair1.first = object1;
    object_pair1.second = object2;

    Object object3;
    object3 << 1;
    object3.set_actual_label(0);
    object3.set_predicted_label(0);
    Object object4;
    object4 << 2;
    object4.set_actual_label(0.5);
    object4.set_predicted_label(3.0);
    object_pair2.first =  object3;
    object_pair2.second =  object4;

    Object object5;
    object5 << 1;
    object5.set_actual_label(-1);
    object5.set_predicted_label(-2);
    Object object6;
    object6 << 2;
    object6.set_actual_label(-1);
    object6.set_predicted_label(-2);
    object_pair3.first = object5;
    object_pair3.second = object6;
  }

  virtual void TearDown() {
  }

  protected:
    ObjectPair object_pair1;
    ObjectPair object_pair2;
    ObjectPair object_pair3;
};

TEST_F(PairwiseMeasuresTest, TestingAccuracy) {
  Accuracy<ObjectPair> ac;
  EXPECT_EQ(0, ac(object_pair1));
  EXPECT_EQ(1, ac(object_pair2));
  EXPECT_EQ(1, ac(object_pair3));
}
