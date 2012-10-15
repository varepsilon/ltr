// Copyright 2011 Yandex School Practice

#include <stdlib.h>
#include <string>

#include "gtest/gtest.h"

#include "ltr/data/object.h"
#include "ltr/data/object_pair.h"
#include "ltr/data/object_list.h"
#include "ltr/data/data_set.h"

using ltr::Object;
using ltr::ObjectPair;
using ltr::ObjectList;
using ltr::DataSet;

// The fixture for testing (contains data for tests).
class ManualSerializationTest : public ::testing::Test {
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

TEST_F(ManualSerializationTest, SerializingObject) {
  Object obj;
  obj << 1 << 2.2444444 << 3.5555;
  obj.set_actual_label(1.0);
  obj.set_predicted_label(2.0);
  EXPECT_EQ(obj.toString(), "[1.00, 2.24, 3.56]{1.00, 2.00}");
}

TEST_F(ManualSerializationTest, SerializingObjectPair) {
  Object obj1;
  Object obj2;
  obj1 << 1 << 2.2444444 << 3.5555;
  obj2 << 3.5555 << 2.2444444 << 1;

  ObjectPair pair(obj1, obj2);
  EXPECT_EQ(pair.toString(), "([1.00, 2.24, 3.56]{0.00, 0.00}, [3.56, 2.24, 1.00]{0.00, 0.00})");
}

TEST_F(ManualSerializationTest, SerializingObjectList) {
  Object obj1;
  Object obj2;
  obj1 << 1 << 2.2444444 << 3.5555;
  obj2 << 3.5555 << 2.2444444 << 1;

  ObjectList lst;
  lst << obj1 << obj2;
  EXPECT_EQ(lst.toString(), "{[1.00, 2.24, 3.56]{0.00, 0.00}, [3.56, 2.24, 1.00]{0.00, 0.00}}");
}

TEST_F(ManualSerializationTest, SerializingDataSet) {
  Object obj1;
  Object obj2;
  obj1 << 1 << 2.2444444 << 3.5555;
  obj2 << 3.5555 << 2.2444444 << 1;

  DataSet<Object> data;
  data << obj1 << obj2;
  EXPECT_EQ(data.toString(), "[1.00, 2.24, 3.56]{0.00, 0.00}\n[3.56, 2.24, 1.00]{0.00, 0.00}\n");
}
