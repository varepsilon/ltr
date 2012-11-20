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
