// Copyright 2011 Yandex

#ifndef LTR_CROSSVALIDATION_SPLITTER_UNITTEST_H_
#define LTR_CROSSVALIDATION_SPLITTER_UNITTEST_H_

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

#endif  // LTR_CROSSVALIDATION_SPLITTER_UNITTEST_H_
