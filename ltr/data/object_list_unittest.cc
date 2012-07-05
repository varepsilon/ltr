// Copyright 2012 Yandex School Practice

#include <stdlib.h>
#include <string>

#include "gtest/gtest.h"

#include "ltr/data/object.h"
#include "ltr/data/object_list.h"

TEST(ObjectListTest, TestObjectListOperations) {
  ltr::Object obj1;
  obj1 << 1 << 2 << 2.57;

  ltr::ObjectList objList1;
  objList1.add(obj1);
  EXPECT_TRUE(objList1[0] == obj1);

  ltr::ObjectList objList2 = objList1.deepCopy();
  EXPECT_EQ(objList1, objList2);

  ltr::Object obj2;
  obj2 << 0 << -3.1;
  objList2.add(obj2);
  EXPECT_EQ(objList1.size(), 1);
  EXPECT_EQ(objList2.size(), 2);
  EXPECT_FALSE(objList1 == objList2);

  objList2 = objList1;
  EXPECT_EQ(objList1, objList2);
  objList2.add(obj2);
  EXPECT_EQ(objList1, objList2);
}