// Copyright 2011 Yandex

#include <vector>
#include <set>

#include <gtest/gtest.h>

#include "ltr/data_preprocessors/data_preprocessor.h"
#include "ltr/data_preprocessors/simple_subset_preprocessor.h"
#include "ltr/data_preprocessors/data_preprocessor_learner.h"
#include "ltr/data_preprocessors/simple_subset_preprocessor_learner.h"
#include "ltr/data_preprocessors/begging_preprocessor_learner.h"

#include "ltr/data/data_set.h"
#include "ltr/data/object.h"

using std::vector;
using std::set;

using ltr::Object;
using ltr::DataSet;
using ltr::DataPreprocessor;
using ltr::SimpleSubsetPreprocessor;
using ltr::SimpleSubsetPreprocessorLearner;
using ltr::BeggingPreprocessorLearner;

const int data_size = 11;

class DataPreprocessorTest : public ::testing::Test {
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

TEST_F(DataPreprocessorTest, SimpleSubsetPreprocessorTest) {
  vector<int> indices;
  indices.push_back(3);
  indices.push_back(7);
  indices.push_back(4);
  SimpleSubsetPreprocessor<Object> prep(indices);

  EXPECT_EQ(3, prep.getChoosedElementsCount());
  EXPECT_EQ(indices, prep.getChoosedElementsIndices());

  DataSet<Object> prep_data;
  prep.apply(data, &prep_data);

  EXPECT_EQ(3, prep_data.size());
  for (int i = 0; i < indices.size(); ++i) {
    EXPECT_EQ(indices[i], prep_data[i].features()[0]);
  }

  indices.push_back(1);
  prep.setChoosedElementsIndices(indices);
  prep.apply(data, &prep_data);

  EXPECT_EQ(4, prep_data.size());
  for (int i = 0; i < indices.size(); ++i) {
    EXPECT_EQ(indices[i], prep_data[i].features()[0]);
  }

  indices.push_back(103);
  prep.setChoosedElementsIndices(indices);
  EXPECT_ANY_THROW(prep.apply(data, &prep_data));
}

TEST_F(DataPreprocessorTest, SimpleSubsetPreprocessorLearnerTest) {
  SimpleSubsetPreprocessorLearner<Object> prep_learner;
  prep_learner.learn(data);
  DataPreprocessor<Object>::Ptr prep = prep_learner.make();

  DataSet<Object> prep_data;
  prep->apply(data, &prep_data);

  vector<int> all_used(data.size());
  for (int index = 0; index < all_used.size(); ++index) {
    all_used[index] = index;
  }

  EXPECT_EQ(all_used.size(), prep_data.size());
  for (int i = 0; i < all_used.size(); ++i) {
    EXPECT_EQ(all_used[i], prep_data[i].features()[0]);
  }

  vector<int> unequal;
  unequal.push_back(3);
  unequal.push_back(5);
  unequal.push_back(3);
  EXPECT_ANY_THROW(prep_learner.setListParameter("INDICES", unequal));

  vector<int> indices;
  indices.push_back(3);
  indices.push_back(5);

  prep_learner.setListParameter("INDICES", indices);
  prep_learner.learn(data);
  DataPreprocessor<Object>::Ptr prep2 = prep_learner.make();

  prep2->apply(data, &prep_data);
  EXPECT_EQ(indices.size(), prep_data.size());
  for (int i = 0; i < indices.size(); ++i) {
    EXPECT_EQ(indices[i], prep_data[i].features()[0]);
  }
}

TEST_F(DataPreprocessorTest, BeggingPreprocessorLearnerTest) {
  BeggingPreprocessorLearner<Object> prep_learner;
  prep_learner.learn(data);
  DataPreprocessor<Object>::Ptr prep = prep_learner.make();

  DataSet<Object> prep_data;
  prep->apply(data, &prep_data);

  EXPECT_EQ(3, prep_data.size());
  for (int i = 0; i < prep_data.size(); ++i) {
    EXPECT_GT(data_size, prep_data[i].features()[0]);
    EXPECT_LE(0, prep_data[i].features()[0]);
  }

  prep_learner.setBoolParameter("WITH_REPLACE", false);
  prep_learner.setDoubleParameter("SELECTED_PART", 0.8);

  prep_learner.learn(data);
  DataPreprocessor<Object>::Ptr prep2 = prep_learner.make();
  prep2->apply(data, &prep_data);

  EXPECT_EQ(8, prep_data.size());
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

  prep_learner.setBoolParameter("WITH_REPLACE", true);
  prep_learner.setDoubleParameter("SELECTED_PART", 1.5);

  prep_learner.learn(data);
  DataPreprocessor<Object>::Ptr prep3 = prep_learner.make();
  prep3->apply(data, &prep_data);

  EXPECT_EQ(16, prep_data.size());
  for (int i = 0; i < prep_data.size(); ++i) {
    EXPECT_GT(data_size, prep_data[i].features()[0]);
    EXPECT_LE(0, prep_data[i].features()[0]);
  }
}
