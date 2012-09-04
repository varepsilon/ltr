// Copyright 2012 Yandex

#include <gtest/gtest.h>
#include <vector>

#include "ltr/utility/numerical.h"
#include "ltr/data/object.h"
#include "ltr/scorers/composition_scorers/linear_composition_scorer.h"
#include "ltr/scorers/linear_scorer.h"
#include "ltr/scorers/one_feature_scorer.h"

using ltr::Object;
using ltr::LinearScorer;
using ltr::composition::LinearCompositionScorer;
using ltr::OneFeatureScorer;
using ltr::utility::DoubleEqual;
using std::vector;

TEST(LinearCompositionScorer, LinearCompositionScorerTest) {
  vector<double> weights1(2);
  weights1[0] = 0.5;
  weights1[1] = 1.7;

  LinearScorer::Ptr ls1(new LinearScorer(weights1));
  OneFeatureScorer::Ptr ofs1(new OneFeatureScorer());

  Object obj1;
  obj1 << 1.2;

  LinearCompositionScorer lcs1;
  lcs1.add(ls1, 5.2);
  lcs1.add(ofs1, 4.2);

  EXPECT_TRUE(DoubleEqual(18.248, lcs1(obj1)));

  vector<double> weights2(5);
  weights2[0] = 0.4;
  weights2[1] = 0.92;
  weights2[2] = 0.003;
  weights2[3] = 0.1;
  weights2[4] = 0.2;

  vector<double> weights3(5);
  weights3[0] = 1.2;
  weights3[1] = 1.7;
  weights3[2] = 1.9;
  weights3[3] = 0.02;
  weights3[4] = 0.07;

  LinearScorer::Ptr ls2(new LinearScorer(weights2));
  LinearScorer::Ptr ls3(new LinearScorer(weights3));

  LinearCompositionScorer lcs2;
  lcs2.add(ls2, 2.1);
  lcs2.add(ls3, 0.7);

  Object obj2;
  obj2 << 1.9 << 3.7 << 0.2 << 4.1;

  EXPECT_TRUE(DoubleEqual(14.52381, lcs2(obj2)));
  EXPECT_EQ(2, lcs2.size());

  LinearScorer::Ptr ls4(new LinearScorer(weights2));
  lcs2.clear();
  lcs2.add(ls4, 1);
  EXPECT_TRUE(DoubleEqual(2.9991, lcs2(obj2)));
  EXPECT_EQ(1, lcs2.size());

  lcs2.toString();
}
