// Copyright 2011 Yandex

#include <gtest/gtest.h>

#include "ltr/data/object.h"
#include "ltr/scorers/one_feature_scorer.h"

using ltr::Object;
using ltr::OneFeatureScorer;

TEST(OneFeatureScorerTest, OneFeatureScorerScoringTest) {
  Object obj;
  obj << 1.2;

  OneFeatureScorer::Ptr one_feature_scorer1(new OneFeatureScorer);
  EXPECT_EQ(1.2, (*one_feature_scorer1)(obj));

  obj << 4;
  EXPECT_EQ(1.2, (*one_feature_scorer1)(obj));

  OneFeatureScorer::Ptr one_feature_scorer2(new OneFeatureScorer(1));
  EXPECT_EQ(4, (*one_feature_scorer2)(obj));
  EXPECT_EQ(1, (*one_feature_scorer2).getIndex());

  obj << 5;
  one_feature_scorer1->setIndex(2);
  EXPECT_EQ(5, (*one_feature_scorer1)(obj));

  one_feature_scorer1->toString();
};
