// Copyright 2012 Yandex School Practice

#include <stdlib.h>
#include <string>

#include "gtest/gtest.h"

#include "ltr/data/data_set.h"
#include "ltr/data/object.h"
#include "ltr/data/object_list.h"
#include "ltr/data/object_pair.h"

TEST(ObjectTest, TestObjectOperations) {
  ltr::Object object1;
  object1 << 1 << 2.15 << 3.167;
  EXPECT_TRUE(object1.feature_count() == 3);

  ltr::Object object2 = object1.deepCopy();
  EXPECT_TRUE(object1 == object2);
  object2 << 14.75;

  EXPECT_FALSE(object1 == object2);
  EXPECT_TRUE(object1.feature_count() == 3);
  EXPECT_TRUE(object2.feature_count() == 4);

  object1.setMetaInfo("someName", "someValue");
  EXPECT_EQ(object1.getMetaInfo("someName"), "someValue");
  EXPECT_ANY_THROW(object1.getMetaInfo("otherName"));
}

