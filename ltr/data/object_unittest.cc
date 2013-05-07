// Copyright 2012 Yandex School Practice

#include <stdlib.h>
#include <string>

#include "gtest/gtest.h"

#include "ltr/data/data_set.h"
#include "ltr/data/object.h"
#include "ltr/data/object_list.h"
#include "ltr/data/object_pair.h"

using ltr::DataSet;

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

TEST(ObjectTest, TestObjectEquality) {
  Object object1, object2, object3, object4;
  DataSet<Object> data_set;
  object1 << 1;
  object2 << 2;
  object3 << 3;
  object4 << 4;

  data_set << object1 << object2 << object3 << object4;

  Object object5 = data_set[0];
  EXPECT_EQ(object5, data_set[0]);
  object5 << 1;
  EXPECT_FALSE(object5 == data_set[0]);

  EXPECT_ANY_THROW(data_set[0] = object5);
  EXPECT_NO_THROW(data_set[0] = object4);

  object3 = object2;
  EXPECT_EQ(object3, object2);
  object2[0] = 100;
  EXPECT_FALSE(object3 == object2);

  EXPECT_NO_THROW(object3.lightCopyFrom(object1));
  EXPECT_EQ(object3, object1);
  object3 << 1;
  EXPECT_EQ(object3, object1);

  EXPECT_NO_THROW(object3.lightCopyFrom(data_set[2]));
  EXPECT_EQ(object3, data_set[2]);

  EXPECT_ANY_THROW(object3 << 1);
  object3.at(0) = -1;
  EXPECT_EQ(object3, data_set[2]);

  EXPECT_ANY_THROW(data_set[2].lightCopyFrom(object3));
}
