// Copyright 2012 Yandex

#include "gtest/gtest.h"

#include "ltr/utility/boost/shared_ptr.h"

using ltr::utility::shared_ptr;

TEST(SharedPtrTest, SharedPtrTest) {
  shared_ptr<int> p0;
  shared_ptr<int> p1 = p0;
  EXPECT_EQ(p0, p1);

  shared_ptr<int> p2(new int(42)); // NOLINT
  shared_ptr<int> p3 = p2;
  EXPECT_EQ(*p2, 42);
  EXPECT_EQ(*p3, 42);

  p3.reset(new int(1)); // NOLINT
  EXPECT_EQ(*p3, 1);

  p0.swap(p2);
  EXPECT_EQ(*p0, 42);

  shared_ptr<int> p4 = NULL;
  shared_ptr<int> p5 = new int(42); // NOLINT
  EXPECT_TRUE(p5);
  EXPECT_FALSE(p4);
}
