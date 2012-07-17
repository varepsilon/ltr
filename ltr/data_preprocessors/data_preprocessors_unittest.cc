// Copyright 2012 Yandex

#include <gtest/gtest.h>

#include <vector>
#include <set>

#include "ltr/data/data_set.h"
#include "ltr/data/object.h"

#include "ltr/data_preprocessors/data_preprocessor.h"
#include "ltr/data_preprocessors/data_sampler.h"
#include "ltr/data_preprocessors/data_random_sampler.h"
#include "ltr/data_preprocessors/fake_data_preprocessor.h"

#include "ltr/utility/indices.h"

using std::vector;
using std::set;

using ltr::Object;
using ltr::DataSet;

using ltr::DataPreprocessor;
using ltr::DataSampler;
using ltr::DataRandomSampler;
using ltr::FakePreprocessor;

using ltr::utility::Indices;
using ltr::utility::IndicesPtr;

const int data_size = 11;

class DataPreprocessorsTest : public ::testing::Test {
  protected:
  virtual void SetUp() {
    for (int index = 0; index < data_size; ++index) {
      Object object;
      object << index;
      data.add(object);
    }
    // note that this dataset has invalid feature info
    // but it's not significant for non-feature data preprocessors
  }

  DataSet<Object> data;
};

TEST_F(DataPreprocessorsTest, CommonTest) {
  FakePreprocessor<Object> fake_preprocessor;
  DataSampler<Object> data_sampler;
  DataRandomSampler<Object> data_random_sampler;

  DataSet<Object> preprocessed_data;

  ASSERT_NO_THROW(fake_preprocessor.apply(data, &preprocessed_data));
  ASSERT_NO_THROW(data_sampler.apply(data, &preprocessed_data));
  ASSERT_NO_THROW(data_random_sampler.apply(data, &preprocessed_data));
}

TEST_F(DataPreprocessorsTest, FakePreprocessorTest) {
  FakePreprocessor<Object> fake_preprocessor;

  DataSet<Object> preprocessed_data;
  fake_preprocessor.apply(data, &preprocessed_data);

  EXPECT_EQ(data, preprocessed_data);
}

TEST_F(DataPreprocessorsTest, DataSamplerTest) {
  IndicesPtr indices(new Indices());
  indices->push_back(3);
  indices->push_back(7);
  indices->push_back(4);

  DataSampler<Object> data_sampler;
  data_sampler.set_indices(indices);

  DataSet<Object> preprocessed_data;
  data_sampler.apply(data, &preprocessed_data);

  EXPECT_EQ(3, preprocessed_data.size());
  EXPECT_EQ(data[3], preprocessed_data[0]);
  EXPECT_EQ(data[7], preprocessed_data[1]);
  EXPECT_EQ(data[4], preprocessed_data[2]);

  indices->push_back(1);
  data_sampler.set_indices(indices);
  data_sampler.apply(data, &preprocessed_data);

  EXPECT_EQ(4, preprocessed_data.size());
  EXPECT_EQ(data[3], preprocessed_data[0]);
  EXPECT_EQ(data[7], preprocessed_data[1]);
  EXPECT_EQ(data[4], preprocessed_data[2]);
  EXPECT_EQ(data[1], preprocessed_data[3]);

  indices->push_back(103);
  data_sampler.set_indices(indices);
  EXPECT_ANY_THROW(data_sampler.apply(data, &preprocessed_data));

  data_sampler.setDefaultParameters();
  data_sampler.apply(data, &preprocessed_data);
  EXPECT_EQ(data, preprocessed_data);
}

TEST_F(DataPreprocessorsTest, DataRandomSamplerTest) {
  DataRandomSampler<Object> data_random_sampler;

  DataSet<Object> preprocessed_data;
  data_random_sampler.apply(data, &preprocessed_data);

  EXPECT_EQ(4, preprocessed_data.size());
  for (int index = 0; index < preprocessed_data.size(); ++index) {
    EXPECT_GT(data_size, preprocessed_data[index][0]);
    EXPECT_LE(0, preprocessed_data[index][0]);
  }

  data_random_sampler.set_with_replacement(false);
  data_random_sampler.set_sampling_fraction(0.8);
  data_random_sampler.apply(data, &preprocessed_data);

  EXPECT_EQ(9, preprocessed_data.size());
  set<int> used_elements;
  for (int index = 0; index < preprocessed_data.size(); ++index) {
    EXPECT_GT(data_size, preprocessed_data[index][0]);
    EXPECT_LE(0, preprocessed_data[index][0]);

    int current_object = preprocessed_data[index][0];
    if (used_elements.find(current_object) == used_elements.end()) {
      used_elements.insert(current_object);
    } else {
      ADD_FAILURE();
    }
  }

  data_random_sampler.set_with_replacement(true);
  data_random_sampler.set_sampling_fraction(1.5);
  data_random_sampler.apply(data, &preprocessed_data);

  EXPECT_EQ(17, preprocessed_data.size());
  for (int index = 0; index < preprocessed_data.size(); ++index) {
    EXPECT_GT(data_size, preprocessed_data[index][0]);
    EXPECT_LE(0, preprocessed_data[index][0]);
  }

  data_random_sampler.set_sampling_fraction(0.);
  EXPECT_ANY_THROW(data_random_sampler.checkParameters());
  data_random_sampler.set_sampling_fraction(0.5);
  data_random_sampler.set_with_replacement(false);
  data_random_sampler.set_sampling_fraction(0.);
  EXPECT_ANY_THROW(data_random_sampler.checkParameters());

  data_random_sampler.set_sampling_fraction(1e-8);
  data_random_sampler.apply(data, &preprocessed_data);
  EXPECT_EQ(1, preprocessed_data.size());
}
