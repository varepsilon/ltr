// Copyright 2013 Yandex

#include "gtest/gtest.h"

#include "ltr/utility/safe_map.hpp"

using ltr::utility::SafeMap;

TEST(MapTest, TestingSafeFind) {
  SafeMap<int, int> test_map;
  test_map[0] = 13;
  test_map[1] = 42;
  SafeMap<int, int>::iterator iter1 = test_map.safeFind(0);
  SafeMap<int, int>::const_iterator iter2 = test_map.safeFind(1);
  EXPECT_EQ(iter1->second, 13);
  EXPECT_EQ(iter2->second, 42);
  EXPECT_ANY_THROW(test_map.safeFind(3));
  EXPECT_ANY_THROW(test_map.safeFind(3)->second = 5);
}

TEST(MapTest, TestingContains) {
  SafeMap<int, int> test_map;
  test_map[0] = 13;
  test_map[1] = 42;
  EXPECT_TRUE(test_map.contains(0));
  EXPECT_FALSE(test_map.contains(2));
}

TEST(MapTest, TestingSafeInsert) {
  SafeMap<int, int> test_map;
  test_map.safeInsert(0) = 13;
  EXPECT_EQ(test_map[0], 13);
  EXPECT_ANY_THROW(test_map.safeInsert(0) = 5);
}

TEST(MapTest, TestingSafeAt) {
  SafeMap<int, int> test_map;
  test_map.safeInsert(0) = 13;
  test_map.safeInsert(1) = 14;
  EXPECT_NO_THROW(test_map.safeAt(0) = 42);
  EXPECT_EQ(test_map.safeAt(0), 42);
  EXPECT_ANY_THROW(test_map.safeAt(2));

  const SafeMap<int, int> const_test_map = test_map;
  EXPECT_EQ(const_test_map.safeAt(0), 42);
  EXPECT_ANY_THROW(const_test_map.safeAt(2));
}
