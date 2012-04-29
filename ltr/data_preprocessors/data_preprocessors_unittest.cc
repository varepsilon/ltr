// Copyright 2011 Yandex

#include <gtest/gtest.h>

#include <vector>
#include <set>

#include "ltr/data_preprocessors/data_preprocessor.h"
#include "ltr/data_preprocessors/simple_subset_preprocessor.h"
#include "ltr/data_preprocessors/begging_preprocessor.h"
#include "ltr/data_preprocessors/fake_preprocessor.h"

#include "ltr/data/data_set.h"
#include "ltr/data/object.h"

using std::vector;
using std::set;

using ltr::Object;
using ltr::DataSet;
using ltr::DataPreprocessor;
using ltr::SimpleSubsetPreprocessor;
using ltr::BeggingPreprocessor;
using ltr::FakePreprocessor;

const int data_size = 11;

class DataPreprocessorsTest : public ::testing::Test {
  protected:
  virtual void SetUp() {
    for (int i = 0; i < data_size; ++i) {
      Object obj;
      obj << i;
      data.add(obj);
    }
    // note that this dataset has invalid feature info
    // but it's not significant for non-feature data preprocessors
  }

  DataSet<Object> data;
};

template<class TElement>
bool AreEqual(const DataSet<TElement>& first,
              const DataSet<TElement>& second) {
  if (first.size() != second.size()) {
    return false;
  }
  for (int i = 0; i < first.size(); ++i) {
    if (first[i].features() != second[i].features()) {
      return false;
    }
  }
  return true;
}

TEST_F(DataPreprocessorsTest, FakePreprocessorTest) {
  FakePreprocessor<Object> prep;

  DataSet<Object> prep_data;
  prep.apply(data, &prep_data);

  EXPECT_TRUE(AreEqual(data, prep_data));
}

TEST_F(DataPreprocessorsTest, SimpleSubsetPreprocessorTest) {
  vector<int> indices;
  indices.push_back(3);
  indices.push_back(7);
  indices.push_back(4);
  SimpleSubsetPreprocessor<Object> prep;
  prep.setListParameter("INDICES", indices);

  DataSet<Object> prep_data;
  prep.apply(data, &prep_data);

  EXPECT_EQ(3, prep_data.size());
  for (int i = 0; i < indices.size(); ++i) {
    EXPECT_EQ(indices[i], prep_data[i].features()[0]);
  }

  indices.push_back(1);
  prep.setListParameter("INDICES", indices);
  prep.apply(data, &prep_data);

  EXPECT_EQ(4, prep_data.size());
  for (int i = 0; i < indices.size(); ++i) {
    EXPECT_EQ(indices[i], prep_data[i].features()[0]);
  }

  indices.push_back(103);
  prep.setListParameter("INDICES", indices);
  EXPECT_ANY_THROW(prep.apply(data, &prep_data));

  prep.setDefaultParameters();
  prep.apply(data, &prep_data);
  EXPECT_TRUE(AreEqual(data, prep_data));

  vector<int> unequal;
  unequal.push_back(3);
  unequal.push_back(5);
  unequal.push_back(3);
  EXPECT_ANY_THROW(prep.setListParameter("INDICES", unequal));
}

TEST_F(DataPreprocessorsTest, BeggingPreprocessorTest) {
  BeggingPreprocessor<Object> prep;

  DataSet<Object> prep_data;
  prep.apply(data, &prep_data);

  EXPECT_EQ(4, prep_data.size());
  for (int i = 0; i < prep_data.size(); ++i) {
    EXPECT_GT(data_size, prep_data[i].features()[0]);
    EXPECT_LE(0, prep_data[i].features()[0]);
  }

  prep.setBoolParameter("WITH_REPLACE", false);
  prep.setDoubleParameter("SELECTED_PART", 0.8);
  prep.apply(data, &prep_data);

  EXPECT_EQ(9, prep_data.size());
  set<int> used_elements;
  for (int i = 0; i < prep_data.size(); ++i) {
    EXPECT_GT(data_size, prep_data[i].features()[0]);
    EXPECT_LE(0, prep_data[i].features()[0]);

    int current_object = prep_data[i].features()[0];
    if (used_elements.find(current_object) == used_elements.end()) {
      used_elements.insert(current_object);
    } else {
      ADD_FAILURE();
    }
  }

  prep.setBoolParameter("WITH_REPLACE", true);
  prep.setDoubleParameter("SELECTED_PART", 1.5);
  prep.apply(data, &prep_data);

  EXPECT_EQ(17, prep_data.size());
  for (int i = 0; i < prep_data.size(); ++i) {
    EXPECT_GT(data_size, prep_data[i].features()[0]);
    EXPECT_LE(0, prep_data[i].features()[0]);
  }

  EXPECT_ANY_THROW(prep.setDoubleParameter("SELECTED_PART", 0.0));
  prep.setDoubleParameter("SELECTED_PART", 0.5);
  prep.setBoolParameter("WITH_REPLACE", false);
  EXPECT_ANY_THROW(prep.setDoubleParameter("SELECTED_PART", 0.0));

  prep.setDoubleParameter("SELECTED_PART", 1e-8);
  prep.apply(data, &prep_data);
  EXPECT_EQ(1, prep_data.size());
}
