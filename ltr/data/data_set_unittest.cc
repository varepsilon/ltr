// Copyright 2011 Yandex School Practice

#include "gtest/gtest.h"

#include "ltr/data/object.h"
#include "ltr/data/data_set.h"

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

TEST_F(DataSetTest, DataSetTest_test) {
  ltr::DataSet<ltr::ObjectList> data_set;
}
