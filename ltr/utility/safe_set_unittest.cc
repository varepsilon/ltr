// Copyright 2013 Yandex

#include "gtest/gtest.h"

#include "ltr/utility/safe_set.hpp"

using ltr::utility::SafeSet;

TEST(SetTest, TestingContains) {
  SafeSet<int> test_set;
  test_set.insert(0);
  test_set.insert(1);
  EXPECT_TRUE(test_set.contains(0));
  EXPECT_FALSE(test_set.contains(2));
}

TEST(SetTest, TestingSafeInsert) {
  SafeSet<int> test_set;
  test_set.safeInsert(0);
  EXPECT_TRUE(test_set.contains(0));
  EXPECT_ANY_THROW(test_set.safeInsert(0));
}
