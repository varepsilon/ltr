// Copyright 2011 Yandex School Practice

#include <gtest/gtest.h>

#include "ltr/measures/measure.h"
#include "ltr/measures/abs_error.h"
#include "ltr/measures/accuracy.h"
#include "ltr/measures/squared_error.h"
#include "ltr/measures/average_precision.h"
#include "ltr/measures/normalized_measure.h"
#include "ltr/data/object.h"
#include "ltr/data/object_list.h"
#include "ltr/data/data_set.h"
#include "ltr/utility/numerical.h"

using ltr::Object;
using ltr::ObjectList;
using ltr::DataSet;
using ltr::FeatureInfo;
using ltr::utility::DoubleEqual;
using ltr::AbsError;
using ltr::SquaredError;
using ltr::AveragePrecision;
using ltr::Accuracy;
using ltr::NormalizedMeasure;

TEST(MeasureTest, MeasureTest) {
  Object object1;
  object1 << 1;
  object1.set_actual_label(0);
  object1.set_predicted_label(2);

  Object object2;
  object2 << 2;
  object2.set_actual_label(1.4);
  object2.set_predicted_label(1.4);

  Object object3;
  object3 << 3;
  object3.set_actual_label(3.5);
  object3.set_predicted_label(-3.5);

  Object object4;
  object4 << 4;
  object4.set_actual_label(0.5);
  object4.set_predicted_label(4.0);

  DataSet<Object> data_set(FeatureInfo(1));
  DataSet<Object> empty_data_set(FeatureInfo(1));
  data_set.add(object1, 1.0);
  data_set.add(object2, 2.0);
  data_set.add(object3, 0.5);
  data_set.add(object4, 1.2);

  AbsError ae;
  EXPECT_TRUE(DoubleEqual(ae.average(data_set), 2.063829787234042));
  EXPECT_ANY_THROW(ae.average(empty_data_set));

  SquaredError se;
  EXPECT_TRUE(DoubleEqual(se.average(data_set), 9.191489361702127));
  EXPECT_ANY_THROW(se.average(empty_data_set));

  ObjectList empty_olist;
  AveragePrecision ap;
  EXPECT_ANY_THROW(ap(empty_olist));

  EXPECT_TRUE(ae.better(0.3, 1.2));
  EXPECT_FALSE(se.better(0.2, 0.1));
  EXPECT_TRUE(ap.better(1, 0.5));
  EXPECT_FALSE(ap.better(0.3, 1.5));
}

TEST(MeasureTest, NormalizedMeasureTest) {
  AbsError::Ptr abs_error(new AbsError());
  NormalizedMeasure<Object> norm(abs_error);
  NormalizedMeasure<Object> norm2(abs_error, 1, -1);
  Object obj;
  obj.set_actual_label(1);
  obj.set_predicted_label(0);

  norm.setWeakMeasure(AbsError());

  EXPECT_THROW(norm.value(obj), std::logic_error);

  norm.setWeakMeasure(Accuracy<Object>());
  norm2.setWeakMeasure(Accuracy<Object>());
  EXPECT_NO_THROW(norm.value(obj));
  EXPECT_EQ(norm.worst(), -1);
  EXPECT_EQ(norm.best(), 1);

  EXPECT_EQ(norm.value(obj), -1);
  EXPECT_EQ(norm2.value(obj), 1);

  obj.set_predicted_label(1);
  EXPECT_EQ(norm.value(obj), 1);
  EXPECT_EQ(norm2.value(obj), -1);
}
