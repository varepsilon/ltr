// Copyright 2012 Yandex School Practice

#include <stdlib.h>
#include <string>

#include "gtest/gtest.h"

#include "ltr/data/object.h"
#include "ltr/data/object_list.h"
#include "ltr/data/utility/data_set_converters.h"

TEST(ListToAllPairsConverterTest, TestListConvertion) {
  ltr::Object object1;
  ltr::Object object2;
  ltr::Object object3;

  object1 << 1 << 2 << 3;
  object2 << 4 << 5 << 6;
  object3 << 7 << 5 << 3.13;

  ltr::ObjectList objList;
  objList << object1 << object2 << object3;

  ltr::utility::ListToAllPairsConverter converter;
  std::vector<ltr::ObjectPair> pairs;

  EXPECT_NO_THROW(converter(objList, &pairs));
  EXPECT_EQ(pairs.size(), 3);
  EXPECT_EQ(pairs[0], ltr::ObjectPair(object2, object1));
  EXPECT_EQ(pairs[1], ltr::ObjectPair(object3, object1));
  EXPECT_EQ(pairs[2], ltr::ObjectPair(object3, object2));
}