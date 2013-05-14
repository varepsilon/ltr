// Copyright 2012 Yandex

#include <stdlib.h>
#include <string>

#include "gtest/gtest.h"

#include "ltr/data/object.h"
#include "ltr/data/data_set.h"

#include "ltr/data/utility/io_utility.h"
#include "ltr/data/utility/data_set_converters.h"

#include "ltr/utility/random_number_generator.h"
#include "ltr/utility/boost/lexical_cast.h"

using ltr::utility::convertDataSet;
using ltr::utility::randomizer;
using ltr::DataSet;
using ltr::Object;


TEST(DataSetTest, DataSetLightSubsetTest) {
  ltr::DataSet<ltr::Object> data_set(ltr::FeatureInfo(3));

  ltr::Object object1;
  ltr::Object object2;
  ltr::Object object3;
  object1 << 1.0 << 2.0 << 3.0;
  object2 << 3.0 << 1.0 << 2.0;
  object3 << 2.0 << 3.0 << 1.0;
  data_set.add(object1);
  data_set.add(object2);
  data_set.add(object3);

  ltr::DataSet<ltr::Object> light_subset =
              data_set.lightSubset(std::vector<int>(1, 1));

  EXPECT_EQ(light_subset.at(0), object2);
}

TEST(DataSetTest, DataSetIteratorsTest) {
  Object object1;
  Object object2;
  Object object3;
  object1 << 1.0 << 2.0 << 3.0;
  object2 << 3.0 << 1.0 << 2.0;
  object3 << 2.0 << 3.0 << 1.0;
  DataSet<Object> data_set;
  data_set.add(object1);
  data_set.add(object2);
  data_set.add(object3);

  const DataSet<Object>& data_set_ref = data_set;

  DataSet<ObjectList> data_set2;
  ObjectList list;
  list.add(object1);
  list.add(object2);
  list.add(object3);

  const DataSet<ObjectList>& data_set_ref2 = data_set2;
  data_set2.add(list);

  DataSet<ObjectPair> data_set3;
  ObjectPair pair1, pair2;

  pair1.first = object1;
  pair1.second = object2;
  pair2.first = object1;
  pair2.second = object3;

  const DataSet<ObjectPair>& data_set_ref3 = data_set3;
  data_set3.add(pair1);
  data_set3.add(pair2);


  for (DataSet<Object>::object_iterator it = data_set.object_begin();
       it != data_set.object_end();
       ++it) {
    Object obj = *it;
    EXPECT_NO_THROW(obj[1] = 1);
    EXPECT_EQ(obj[1], 1);
  }

  for (DataSet<Object>::const_object_iterator it = data_set_ref.object_begin();
       it != data_set_ref.object_end();
       ++it) {
    Object obj;
    EXPECT_NO_THROW(obj = *it);
  }

  for (DataSet<ObjectList>::element_iterator it = data_set2.element_begin();
       it != data_set2.element_end();
       ++it) {
    ObjectList list = *it;
    EXPECT_NO_THROW(list.at(1)[1] = 1);
  }

  for (DataSet<ObjectList>::const_element_iterator
       it = data_set_ref2.element_begin();
       it != data_set_ref2.element_end();
       ++it) {
    ObjectList list;
    EXPECT_NO_THROW(list = *it);
  }

  for (DataSet<ObjectPair>::object_iterator it = data_set3.object_begin();
       it != data_set3.object_end();
       ++it) {
    Object obj = *it;
    EXPECT_NO_THROW(obj[1] = 1);
    EXPECT_EQ(obj[1], 1);
  }

  for (DataSet<ObjectPair>::const_element_iterator
       it = data_set_ref3.element_begin();
       it != data_set_ref3.element_end();
       ++it) {
    ObjectList list;
    EXPECT_NO_THROW(pair1 = *it);
    EXPECT_EQ(object1, pair1.first);
  }
}


TEST(DataSetTest, DataSetConvertersTest) {
  const int number_of_features = 3;
  const int max_list_size = 5;
  const int min_list_size = 4;
  const double rand_normalizer = 15332;
  ltr::DataSet<ltr::ObjectList> list_data;
  int number_of_objects = randomizer.rand() %
    (1 + max_list_size - min_list_size) + min_list_size;

  ltr::ObjectList list;
  for (int object_index = 0; object_index < number_of_objects; object_index++) {
    ltr::Object object;
    object.setMetaInfo("queryId", ltr::utility::lexical_cast<std::string>(0));
    for (int feature_index = 0;
         feature_index < number_of_features; feature_index++) {
      object << (double)randomizer.rand() / rand_normalizer;
    }
    list << object;
  }
  list_data << list;

  EXPECT_NO_THROW(convertDataSet<ltr::Object>(list_data));
  ltr::DataSet<ltr::ObjectPair> pair_data;
  EXPECT_NO_THROW(pair_data = convertDataSet<ltr::ObjectPair>(list_data));
  EXPECT_NO_THROW(convertDataSet<ltr::Object>(pair_data));
}
