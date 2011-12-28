// Copyright 2011 Yandex School Practice

#include "gtest/gtest.h"

#include "ltr/data/object.h"
#include "ltr/data/data_set.h"
#include "ltr/data/utility/data_set_utility.h"

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
      std::vector<size_t>(1, 1));

  EXPECT_EQ(light_subset.at(0), o2);
}
