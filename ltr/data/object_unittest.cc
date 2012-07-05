// Copyright 2012 Yandex School Practice

#include <stdlib.h>
#include <string>

#include "gtest/gtest.h"

#include "ltr/data/object.h"

class ObjectTest : public ::testing::Test {
    protected:
    virtual void SetUp() {
        // Code here will be called immediately after the constructor (right
        // before each test).
    }

    virtual void TearDown() {
        // Code here will be called immediately after each test (right
        // before the destructor).
    }
};

TEST(ObjectTest, TestObjectOperations) {
  ltr::Object obj1;
  obj1 << 1 << 2.15 << 3.167;
  EXPECT_TRUE(obj1.feature_count() == 3);

  ltr::Object obj2 = obj1.deepCopy();
  EXPECT_TRUE(obj1 == obj2);
  obj2 << 14.75;

  EXPECT_FALSE(obj1 == obj2);
  EXPECT_TRUE(obj1.feature_count() == 3);
  EXPECT_TRUE(obj2.feature_count() == 4);

  obj1.setMetaInfo("someName", "someValue");
  EXPECT_EQ(obj1.getMetaInfo("someName"), "someValue");
  EXPECT_ANY_THROW(obj1.getMetaInfo("otherName"));
}