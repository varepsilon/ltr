// Copyright 2011 Yandex School Practice

#include <gtest/gtest.h>

#include "ltr/data/object.h"
#include "ltr/data/data_set.h"

#include "ltr/learners/linear_learner/linear_learner.h"

#include "ltr/utility/numerical.h"

using ltr::Object;
using ltr::DataSet;
using ltr::LinearLearner;
using ltr::LinearScorer;

using ltr::utility::DoubleEqual;

TEST(LinearLearnerTest, BasicTest) {
  DataSet<Object> data;

  Object object1, object2, object3;
  object1 << 1;
  object1.set_actual_label(2);
  object2 << 2;
  object2.set_actual_label(4);
  object3 << 3;
  object3.set_actual_label(6);

  data.add(object1);
  data.add(object2);

  LinearLearner<Object>::Ptr llearner(new LinearLearner<Object>);
  llearner->learn(data);
  LinearScorer::Ptr scorer = llearner->makeSpecific();

  EXPECT_TRUE(DoubleEqual(scorer->score(object3), object3.actual_label()));
}

TEST(LinearLearnerTest, ShiftTest) {
  DataSet<Object> data;

  Object object1, object2, object3;
  object1 << 1;
  object1.set_actual_label(3);
  object2 << 2;
  object2.set_actual_label(5);
  object3 << 3;
  object3.set_actual_label(7);

  data.add(object1);
  data.add(object2);

  LinearLearner<Object>::Ptr llearner(new LinearLearner<Object>);
  llearner->learn(data);
  LinearScorer::Ptr scorer = llearner->makeSpecific();

  EXPECT_TRUE(DoubleEqual(scorer->score(object3), object3.actual_label()));
}

TEST(LinearLearnerTest, TwoDimensionalTest) {
  DataSet<Object> data;

  Object object1, object2, object3, object4;
  object1 << 1 << 2;
  object1.set_actual_label(4);
  object2 << 2 << 3;
  object2.set_actual_label(7);
  object3 << 0 << -1;
  object3.set_actual_label(-1);
  object4 << -2 << 5;
  object4.set_actual_label(1);

  data.add(object1);
  data.add(object2);
  data.add(object3);

  LinearLearner<Object>::Ptr llearner(new LinearLearner<Object>);
  llearner->learn(data);
  LinearScorer::Ptr scorer = llearner->makeSpecific();

  EXPECT_TRUE(DoubleEqual(scorer->score(object4), object4.actual_label()));
}

TEST(LinearLearnerTest, TwoDimensionalShiftTest) {
  DataSet<Object> data;

  Object object1, object2, object3, object4;
  object1 << 1 << 2;
  object1.set_actual_label(2);
  object2 << 2 << 3;
  object2.set_actual_label(5);
  object3 << 0 << -1;
  object3.set_actual_label(-3);
  object4 << -2 << 5;
  object4.set_actual_label(-1);

  data.add(object1);
  data.add(object2);
  data.add(object3);

  LinearLearner<Object>::Ptr llearner(new LinearLearner<Object>);
  llearner->learn(data);
  LinearScorer::Ptr scorer = llearner->makeSpecific();

  EXPECT_TRUE(DoubleEqual(scorer->score(object4), object4.actual_label()));
}
