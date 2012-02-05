// Copyright 2011 Yandex

#ifndef LTR_CROSSVALIDATION_SPLITTER_UNITTEST_H_
#define LTR_CROSSVALIDATION_SPLITTER_UNITTEST_H_

#include <gtest/gtest.h>

#include "ltr/data/data_set.h"
#include "ltr/data/object.h"
#include "ltr/data/feature_info.h"

using ltr::DataSet;
using ltr::Object;
using ltr::FeatureInfo;

const int data_lenght = 11;

class SplitterTest : public ::testing::Test {
  protected:
  virtual void SetUp() {
    for (int i = 0; i < data_lenght; ++i) {
      Object obj;
      obj << i;
      data.add(obj);
    }
    // note that this dataset has invalid feature info
    // but it's not significant for splitters
  }

  protected:
  DataSet<Object> data;
};

#endif  // LTR_CROSSVALIDATION_SPLITTER_UNITTEST_H_
