// Copyright 2011 Yandex

#include <gtest/gtest.h>

#include "ltr/data/data_set.h"
#include "ltr/data/object.h"

using ltr::DataSet;
using ltr::Object;

class SplitterTest : public ::testing::Test {
  protected:
  virtual void SetUp() {
    int data_lenght = 11;
    for (int i = 0; i < data_lenght; ++i) {
      Object obj;
      obj << i;
      data.add(obj);
    }
  }

  protected:
  DataSet<Object> data;
};