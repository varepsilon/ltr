// Copyright 2012 Yandex School Practice

#include "gtest/gtest.h"

#include "ltr/data/object.h"
#include "ltr/data/object_list.h"

using ltr::Object;
using ltr::ObjectList;

TEST(ObjectListTest, TestObjectListOperations) {
  Object object1;
  object1 << 1 << 2 << 2.57;

  ObjectList object_list;
  object_list.add(object1);
  EXPECT_TRUE(object_list[0] == object1);

  ObjectList object_list2 = object_list.deepCopy();
  EXPECT_EQ(object_list, object_list2);

  Object object2;
  object2 << 0 << -3.1;
  object_list2.add(object2);
  EXPECT_EQ(object_list.size(), 1);
  EXPECT_EQ(object_list2.size(), 2);
  EXPECT_FALSE(object_list == object_list2);

  object_list2 = object_list;
  EXPECT_EQ(object_list, object_list2);
  object_list2.add(object2);
  EXPECT_EQ(object_list, object_list2);
}