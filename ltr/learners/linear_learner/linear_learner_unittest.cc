// Copyright 2011 Yandex School Practice

#include <gtest/gtest.h>

#include "ltr/data/object.h"
#include "ltr/data/data_set.h"
#include "ltr/learners/linear_learner/linear_learner.h"
#include "ltr/utility/numerical.h"

using ltr::LinearLearner;
using ltr::LinearScorer;
using ltr::Object;
using ltr::DataSet;
using ltr::utility::DoubleEqual;

TEST(LinearLearnerTest, BasicTest) {
  DataSet<Object> data;

  Object o1, o2, o3;
  o1 << 1;
  o1.set_actual_label(2);
  o2 << 2;
  o2.set_actual_label(4);
  o3 << 3;
  o3.set_actual_label(6);

  data.add(o1);
  data.add(o2);

  LinearLearner<Object> llearner;
  llearner.learn(data);
  LinearScorer::Ptr scorer = llearner.makeSpecific();

  EXPECT_TRUE(DoubleEqual(scorer->score(o3), o3.actual_label()));
}

TEST(LinearLearnerTest, ShiftTest) {
  DataSet<Object> data;

  Object o1, o2, o3;
  o1 << 1;
  o1.set_actual_label(3);
  o2 << 2;
  o2.set_actual_label(5);
  o3 << 3;
  o3.set_actual_label(7);

  data.add(o1);
  data.add(o2);

  LinearLearner<Object> llearner;
  llearner.learn(data);
  LinearScorer::Ptr scorer = llearner.makeSpecific();

  EXPECT_TRUE(DoubleEqual(scorer->score(o3), o3.actual_label()));
}

TEST(LinearLearnerTest, TwoDimensionalTest) {
  DataSet<Object> data;

  Object o1, o2, o3, o4;
  o1 << 1 << 2;
  o1.set_actual_label(4);
  o2 << 2 << 3;
  o2.set_actual_label(7);
  o3 << 0 << -1;
  o3.set_actual_label(-1);
  o4 << -2 << 5;
  o4.set_actual_label(1);

  data.add(o1);
  data.add(o2);
  data.add(o3);

  LinearLearner<Object> llearner;
  llearner.learn(data);
  LinearScorer::Ptr scorer = llearner.makeSpecific();

  EXPECT_TRUE(DoubleEqual(scorer->score(o4), o4.actual_label()));
}

TEST(LinearLearnerTest, TwoDimensionalShiftTest) {
  DataSet<Object> data;

  Object o1, o2, o3, o4;
  o1 << 1 << 2;
  o1.set_actual_label(2);
  o2 << 2 << 3;
  o2.set_actual_label(5);
  o3 << 0 << -1;
  o3.set_actual_label(-3);
  o4 << -2 << 5;
  o4.set_actual_label(-1);

  data.add(o1);
  data.add(o2);
  data.add(o3);

  LinearLearner<Object> llearner;
  llearner.learn(data);
  LinearScorer::Ptr scorer = llearner.makeSpecific();

  EXPECT_TRUE(DoubleEqual(scorer->score(o4), o4.actual_label()));
}
