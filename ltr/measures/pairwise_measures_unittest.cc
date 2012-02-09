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
    Object o1;
    o1 << 1;
    o1.setActualLabel(1);
    o1.setPredictedLabel(2);
    Object o2;
    o2 << 2;
    o2.setActualLabel(2);
    o2.setPredictedLabel(1);
    opair1.first() = o1;
    opair1.second() = o2;

    Object o3;
    o3 << 1;
    o3.setActualLabel(0);
    o3.setPredictedLabel(0);
    Object o4;
    o4 << 2;
    o4.setActualLabel(0.5);
    o4.setPredictedLabel(3.0);
    opair2.first() =  o3;
    opair2.second() =  o4;

    Object o5;
    o5 << 1;
    o5.setActualLabel(-1);
    o5.setPredictedLabel(-2);
    Object o6;
    o6 << 2;
    o6.setActualLabel(-1);
    o6.setPredictedLabel(-2);
    opair3.first() = o5;
    opair3.second() = o6;
  }

  virtual void TearDown() {
  }

  protected:
    ObjectPair opair1;
    ObjectPair opair2;
    ObjectPair opair3;
};

TEST_F(PairwiseMeasuresTest, TestingAccuracy) {
  Accuracy<ObjectPair> ac;
  EXPECT_EQ(0, ac(opair1));
  EXPECT_EQ(1, ac(opair2));
  EXPECT_EQ(1, ac(opair3));
}
