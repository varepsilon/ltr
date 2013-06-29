// Copyright 2012 Yandex

#include <gtest/gtest.h>
#include <vector>

#include "ltr/utility/numerical.h"
#include "ltr/data/object.h"
#include "ltr/scorers/linear_scorer.h"

using ltr::Object;
using ltr::DataSet;
using ltr::LinearScorer;
using ltr::utility::DoubleEqual;
using std::vector;

TEST(LinearScorer, LinearScorerTest) {
  Object obj1;
  obj1 << 1.2 << 5.7 << 4.9;

  vector<double> weights1(4);
  weights1[0] = 0.5;
  weights1[1] = 1.7;
  weights1[2] = 4.1;
  weights1[3] = 0.3;

  LinearScorer::Ptr linear_scorer1(new LinearScorer(weights1));
  EXPECT_TRUE(DoubleEqual(27.38, (*linear_scorer1)(obj1)));

  Object obj2;
  obj2 << 0 << 1;

  vector<double> weights2(3);
  weights2[0] = 700;
  weights2[1] = 0.5;
  weights2[2] = 0.03;

  LinearScorer::Ptr linear_scorer2(new LinearScorer(weights2));
  EXPECT_TRUE(DoubleEqual(700.03, (*linear_scorer2)(obj2)));

  obj2.toString();
}
