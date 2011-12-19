// Copyright 2011 Yandex

#include <gtest/gtest.h>

#include "ltr/data/object.h"
#include "ltr/scorers/fake_scorer.h"

using ltr::Object;
using ltr::FakeScorer;

TEST(FakeScorerTest, FakeScorerScoringTest) {
  Object obj;
  obj << 1.2;

  FakeScorer fs;
  EXPECT_EQ(0, fs(obj));

  obj << 4;
  EXPECT_EQ(0, fs(obj));
  EXPECT_EQ(0, fs.score(obj));

  fs.brief();
};
