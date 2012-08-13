// Copyright 2012 Yandex

#include <string>
#include <stdexcept>

#include "gtest/gtest.h"

#include "ltr/data/object.h"
#include "ltr/data/object_pair.h"
#include "ltr/data/data_set.h"
#include "ltr/utility/two_class_predicted_label_converter.h"

using ltr::DataSet;
using ltr::Object;
using ltr::ObjectPair;
using ltr::utility::TwoClassPredictedLabelConverter;

TEST(TwoClassPredictedLabelConverterTest,
  TestingTwoClassPredictedLabelConverter) {
  Object object1, object2, object3, object4, object5;
  object1.set_predicted_label(-2);
  object2.set_predicted_label(300);
  object3.set_predicted_label(31);
  object4.set_predicted_label(-79);
  object5.set_predicted_label(3);
  DataSet<Object> data_set;
  data_set.add(object1);
  data_set.add(object2);
  data_set.add(object3);
  data_set.add(object4);
  data_set.add(object5);

  TwoClassPredictedLabelConverter<Object>
    predicted_label_converter;
  predicted_label_converter.convert(&data_set);
  EXPECT_EQ(-1, data_set[0].predicted_label());
  EXPECT_EQ(1, data_set[1].predicted_label());
  EXPECT_EQ(1, data_set[2].predicted_label());
  EXPECT_EQ(-1, data_set[3].predicted_label());
  EXPECT_EQ(1, data_set[4].predicted_label());
}

TEST(TwoClassPreditcedLabelConverterTest,
  TestingTwoClassPredictedLabelConverterWithPair) {
  ObjectPair object_pair1, object_pair2;
  Object object1, object2, object3, object4;
  object1.set_predicted_label(11);
  object2.set_predicted_label(39);
  object3.set_predicted_label(-52);
  object4.set_predicted_label(43);
  object_pair1 = ObjectPair(object1, object2);
  object_pair2 = ObjectPair(object4, object3);
  DataSet<ObjectPair> data_set;
  data_set.add(object_pair1);
  data_set.add(object_pair2);
  TwoClassPredictedLabelConverter<ObjectPair> predicted_label_converter;
  predicted_label_converter.convert(&data_set);
  EXPECT_EQ(1, data_set[0].first.predicted_label());
  EXPECT_EQ(1, data_set[0].second.predicted_label());
  EXPECT_EQ(1, data_set[1].first.predicted_label());
  EXPECT_EQ(-1, data_set[1].second.predicted_label());
}
