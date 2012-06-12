// Copyright 2011 Yandex School Practice

#include <boost/filesystem/path.hpp>

#include <stdlib.h>
#include <string>

#include "gtest/gtest.h"

#include "ltr/data/object.h"
#include "ltr/data/data_set.h"
#include "ltr/data/utility/data_set_utility.h"
#include "ltr/data/utility/io_utility.h"

#include "ltr/data/utility/data_set_converters.h"

using ltr::utility::convertDataSet;

// The fixture for testing (contains data for tests).
class DataSetTest : public ::testing::Test {
    protected:
    virtual void SetUp() {
        // Code here will be called immediately after the constructor (right
        // before each test).
    }

    virtual void TearDown() {
        // Code here will be called immediately after each test (right
        // before the destructor).
    }
};

// tests.

TEST_F(DataSetTest, DataSetLightSubsetTest) {
  ltr::DataSet<ltr::Object> data_set(ltr::FeatureInfo(3));

  ltr::Object o1;
  ltr::Object o2;
  ltr::Object o3;
  o1 << 1.0 << 2.0 << 3.0;
  o2 << 3.0 << 1.0 << 2.0;
  o3 << 2.0 << 3.0 << 1.0;
  data_set.add(o1);
  data_set.add(o2);
  data_set.add(o3);

  ltr::DataSet<ltr::Object> light_subset = ltr::utility::lightSubset(data_set,
      std::vector<int>(1, 1));

  EXPECT_EQ(light_subset.at(0), o2);
}

TEST_F(DataSetTest, DataSetConvertersTest) {
  const int N = 1;
  const int featureN = 3;
  const int max_list_size = 5;
  const int min_list_size = 4;
  ltr::DataSet<ltr::ObjectList> list_data;
  for (int i = 0; i < N; i++) {
    int cn = rand() % (1 + max_list_size - min_list_size) + min_list_size;
    ltr::ObjectList lst;
    for (int j = 0; j < cn; j++) {
      ltr::Object obj;
      obj.setMetaInfo("queryId", boost::lexical_cast<std::string>(i));
      for (int j = 0; j < featureN; j++)
        obj << static_cast<double>(rand()) / 15332;
      lst << obj;
    }
    list_data << lst;
  }

  EXPECT_NO_THROW(convertDataSet<ltr::Object>(list_data));
  ltr::DataSet<ltr::ObjectPair> pair_data;
  EXPECT_NO_THROW(pair_data = convertDataSet<ltr::ObjectPair>(list_data));
  EXPECT_NO_THROW(convertDataSet<ltr::Object>(pair_data));
}

/*
TEST_F(DataSetTest, DataSetAlignTest) {
  ltr::DataSet<ltr::Object> test_data; 

  std::string test_data_file_name = //"data/imat2009/imat2009_test_small.txt";
      boost::filesystem::path("data/imat2009/imat2009_test_small.txt")
      .string();

  test_data = ltr::io_utility::loadDataSet<ltr::Object>(test_data_file_name,
      "YANDEX");

  for (size_t objIdx = 0; objIdx < test_data.size(); ++objIdx) {
    EXPECT_EQ(test_data.feature_count(), test_data.at(objIdx).feature_count());
  }
}*/
