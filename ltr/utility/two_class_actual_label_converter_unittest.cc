// Copyright 2012 Yandex

#include <string>
#include <stdexcept>

#include "gtest/gtest.h"

#include "ltr/data/object.h"
#include "ltr/data/object_pair.h"
#include "ltr/data/data_set.h"
#include "ltr/utility/two_class_actual_label_converter.h"

using ltr::DataSet;
using ltr::Object;
using ltr::ObjectPair;
using ltr::utility::TwoClassActualLabelConverter;

TEST(TwoClassActualLabelConverterTest, TestingTwoClassActualLabelConverter) {
  Object object1, object2, object3, object4, object5;
  object1.set_actual_label(2);
  object2.set_actual_label(3);
  object3.set_actual_label(3);
  object4.set_actual_label(2);
  object5.set_actual_label(3);
  DataSet<Object> data_set;
  data_set.add(object1);
  data_set.add(object2);
  data_set.add(object3);
  data_set.add(object4);
  data_set.add(object5);

  TwoClassActualLabelConverter<Object> actual_label_converter;
  actual_label_converter.convert(&data_set);
  EXPECT_EQ(-1, data_set[0].actual_label());
  EXPECT_EQ(1, data_set[1].actual_label());
  EXPECT_EQ(1, data_set[2].actual_label());
  EXPECT_EQ(-1, data_set[3].actual_label());
  EXPECT_EQ(1, data_set[4].actual_label());
}

TEST(TwoClassActualLabelConverterTest,
  TestingTwoClassActualLabelConverterExceptions) {
  Object object1, object2, object3;
  object1.set_actual_label(1);
  object2.set_actual_label(2);
  object3.set_actual_label(3);

  DataSet<Object> data_set;
  data_set.add(object1);
  data_set.add(object2);
  data_set.add(object3);
  TwoClassActualLabelConverter<Object> actual_label_converter;
  EXPECT_ANY_THROW(actual_label_converter.convert(&data_set));
}

TEST(TwoClassActualLabelConverterTest,
  TestingTwoClassActualLabelConverterWithPair) {
  ObjectPair object_pair1, object_pair2;
  Object object1, object2, object3, object4;
  object1.set_actual_label(3);
  object2.set_actual_label(3);
  object3.set_actual_label(-50);
  object4.set_actual_label(3);
  object_pair1 = ObjectPair(object1, object2);
  object_pair2 = ObjectPair(object4, object3);
  DataSet<ObjectPair> data_set;
  data_set.add(object_pair1);
  data_set.add(object_pair2);
  TwoClassActualLabelConverter<ObjectPair> actual_label_converter;
  actual_label_converter.convert(&data_set);
  EXPECT_EQ(1, data_set[0].first.actual_label());
  EXPECT_EQ(1, data_set[0].second.actual_label());
  EXPECT_EQ(1, data_set[1].first.actual_label());
  EXPECT_EQ(-1, data_set[1].second.actual_label());
}
