// Copyright 2011 Yandex

#include <gtest/gtest.h>

#include "ltr/data/object.h"
#include "ltr/scorers/one_feature_scorer.h"

using ltr::Object;
using ltr::OneFeatureScorer;

TEST(OneFeatureScorerTest, OneFeatureScorerScoringTest) {
  Object obj;
  obj << 1.2;

  OneFeatureScorer ofs1;
  EXPECT_EQ(1.2, ofs1(obj));

  obj << 4;
  EXPECT_EQ(1.2, ofs1(obj));

  OneFeatureScorer ofs2(1);
  EXPECT_EQ(4, ofs2(obj));
  EXPECT_EQ(1, ofs2.getIndex());

  obj << 5;
  ofs1.setIndex(2);
  EXPECT_EQ(5, ofs1(obj));

  ofs1.brief();
};
