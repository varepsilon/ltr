// Copyright 2012 Yandex

#include <gtest/gtest.h>

#include "ltr/data/data_set.h"
#include "ltr/data/object.h"
#include "ltr/data/feature_info.h"
#include "ltr/utility/indices.h"

#include "ltr/feature_converters/feature_converter.h"
#include "ltr/feature_converters/fake_feature_converter_learner.h"
#include "ltr/feature_converters/feature_sampler_learner.h"
#include "ltr/feature_converters/feature_random_sampler_learner.h"
#include "ltr/feature_converters/feature_normalizer_learner.h"
#include "ltr/feature_converters/nan_to_zero_converter.h"
#include "ltr/feature_converters/nominal_to_bool_converter.h"
#include "ltr/feature_converters/remove_nominal_converter.h"

using ltr::Object;
using ltr::DataSet;
using ltr::FeatureInfo;
using ltr::utility::DoubleEqual;
using ltr::utility::IndicesPtr;
using ltr::utility::Indices;

using ltr::FeatureConverter;
using ltr::FakeFeatureConverterLearner;
using ltr::FeatureSamplerLearner;
using ltr::FeatureRandomSamplerLearner;
using ltr::FeatureNormalizerLearner;
using ltr::NanToZeroConverterLearner;
using ltr::NominalToBoolConverterLearner;
using ltr::RemoveNominalConverterLearner;

const int features_count = 11;

class FeatureConvertersLearnerTest : public ::testing::Test {
 public:
  FeatureConvertersLearnerTest()
    : data(FeatureInfo(features_count)) {}
 protected:
  virtual void SetUp() {
    Object object;
    for (int feature_index = 0;
         feature_index < features_count;
         ++feature_index) {
      object << feature_index;
    }
    data.add(object);
  }

  DataSet<Object> data;
};

TEST_F(FeatureConvertersLearnerTest, CommonTest) {
  FakeFeatureConverterLearner<Object> fake_feature_learner;
  FeatureNormalizerLearner<Object> feature_normalizer_learner;
  FeatureRandomSamplerLearner<Object> feature_random_sampler_learner;
  FeatureSamplerLearner<Object> feature_sampler_learner;
  NanToZeroConverterLearner<Object> nan_to_zero_learner;
  NominalToBoolConverterLearner<Object> nominal_to_bool_learner;
  RemoveNominalConverterLearner<Object> remove_nominal_learner;

  ASSERT_NO_THROW(fake_feature_learner.learn(data));
  ASSERT_NO_THROW(feature_normalizer_learner.learn(data));
  ASSERT_NO_THROW(feature_random_sampler_learner.learn(data));
  ASSERT_NO_THROW(feature_sampler_learner.learn(data));
  ASSERT_NO_THROW(nan_to_zero_learner.learn(data));
  ASSERT_NO_THROW(nominal_to_bool_learner.learn(data));
  ASSERT_NO_THROW(remove_nominal_learner.learn(data));

  FeatureConverter::Ptr fake_feature_converter =
    fake_feature_learner.make();
  FeatureConverter::Ptr feature_normalizer_converter =
    feature_normalizer_learner.make();
  FeatureConverter::Ptr feature_random_sampler_converter =
    feature_random_sampler_learner.make();
  FeatureConverter::Ptr feature_sampler_converter =
    feature_sampler_learner.make();
  FeatureConverter::Ptr nan_to_zero_converter =
    nan_to_zero_learner.make();
  FeatureConverter::Ptr nominal_to_bool_converter =
    nominal_to_bool_learner.make();
  FeatureConverter::Ptr remove_nominal_converter =
    remove_nominal_learner.make();

  DataSet<Object> converted_data;

  ASSERT_NO_THROW(fake_feature_converter->apply(data, &converted_data));
  ASSERT_NO_THROW(feature_normalizer_converter->apply(data, &converted_data));
  ASSERT_NO_THROW(feature_random_sampler_converter->apply(data,
                                                          &converted_data));
  ASSERT_NO_THROW(feature_sampler_converter->apply(data, &converted_data));
  ASSERT_NO_THROW(nan_to_zero_converter->apply(data, &converted_data));
  ASSERT_NO_THROW(nominal_to_bool_converter->apply(data, &converted_data));
  ASSERT_NO_THROW(remove_nominal_converter->apply(data, &converted_data));
}

TEST_F(FeatureConvertersLearnerTest, FeatureSamplerLearnerTest) {
  FeatureSamplerLearner<Object> feature_sampler_learner;
  feature_sampler_learner.learn(data);
  FeatureConverter::Ptr converter = feature_sampler_learner.make();

  DataSet<Object> converted_data;
  converter->apply(data, &converted_data);

  EXPECT_EQ(data, converted_data);

  IndicesPtr indices(new Indices);
  indices->push_back(3);
  indices->push_back(5);

  feature_sampler_learner.set_indices(indices);
  feature_sampler_learner.learn(data);
  FeatureConverter::Ptr converter2 = feature_sampler_learner.make();

  converter2->apply(data, &converted_data);
  EXPECT_EQ(indices->size(), converted_data.feature_count());
  for (int index = 0; index < indices->size(); ++index) {
    EXPECT_EQ((*indices)[index], converted_data[0].features()[index]);
  }
}

TEST_F(FeatureConvertersLearnerTest, FeatureRandomSamplerLearnerTest) {
  FeatureRandomSamplerLearner<Object> feature_random_sampler_learner;
  feature_random_sampler_learner.learn(data);
  FeatureConverter::Ptr converter = feature_random_sampler_learner.make();

  DataSet<Object> converted_data;
  converter->apply(data, &converted_data);

  EXPECT_EQ(4, converted_data.feature_count());
  set<int> used_features;
  for (int feature_index = 0;
       feature_index < converted_data.feature_count();
       ++feature_index) {
    EXPECT_GT(features_count, converted_data[0].features()[feature_index]);
    EXPECT_LE(0, converted_data[0].features()[feature_index]);

    int current_feature = converted_data[0].features()[feature_index];
    if (used_features.find(current_feature) == used_features.end()) {
      used_features.insert(current_feature);
    } else {
      ADD_FAILURE();
    }
  }

  feature_random_sampler_learner.set_sampling_fraction(0.8);
  feature_random_sampler_learner.learn(data);
  FeatureConverter::Ptr converter2 = feature_random_sampler_learner.make();
  converter2->apply(data, &converted_data);

  EXPECT_EQ(9, converted_data.feature_count());
  used_features.clear();
  for (int feature_index = 0;
       feature_index < converted_data.feature_count();
       ++feature_index) {
    EXPECT_GT(features_count, converted_data[0].features()[feature_index]);
    EXPECT_LE(0, converted_data[0].features()[feature_index]);

    int current_feature = converted_data[0].features()[feature_index];
    if (used_features.find(current_feature) == used_features.end()) {
      used_features.insert(current_feature);
    } else {
      ADD_FAILURE();
    }
  }

  feature_random_sampler_learner.set_sampling_fraction(0.0);
  EXPECT_ANY_THROW(feature_random_sampler_learner.learn(data));
  feature_random_sampler_learner.set_sampling_fraction(1e-8);
  feature_random_sampler_learner.learn(data);
  FeatureConverter::Ptr converter3 = feature_random_sampler_learner.make();
  converter3->apply(data, &converted_data);
  EXPECT_EQ(1, converted_data.feature_count());
}

TEST_F(FeatureConvertersLearnerTest, FeatureNormalizerLearnerDefaultTest) {
  FeatureNormalizerLearner<Object> feature_normalizer_learner;

  DataSet<Object> train_data(FeatureInfo(3));
  Object object1, object2, object3, object4;
  object1 << 1.0 << 2.0 << 3.0;
  object2 << -2.0 << 1.0 << 1.0;
  object3 << 0.0 << 5.0 << 2.0;
  object4 << 1.0 << 3.0 << -1.0;
  train_data.add(object1);
  train_data.add(object2);
  train_data.add(object3);
  train_data.add(object4);

  feature_normalizer_learner.learn(train_data);
  FeatureConverter::Ptr converter = feature_normalizer_learner.make();

  DataSet<Object> converted_data;
  converter->apply(train_data, &converted_data);

  EXPECT_TRUE(DoubleEqual(converted_data[0].features()[0], 1.0));
  EXPECT_TRUE(DoubleEqual(converted_data[1].features()[0], 0.0));
  EXPECT_TRUE(DoubleEqual(converted_data[2].features()[0], 0.66666666666666));
  EXPECT_TRUE(DoubleEqual(converted_data[3].features()[0], 1.0));

  EXPECT_TRUE(DoubleEqual(converted_data[0].features()[1], 0.25));
  EXPECT_TRUE(DoubleEqual(converted_data[1].features()[1], 0.0));
  EXPECT_TRUE(DoubleEqual(converted_data[2].features()[1], 1.0));
  EXPECT_TRUE(DoubleEqual(converted_data[3].features()[1], 0.5));

  EXPECT_TRUE(DoubleEqual(converted_data[0].features()[2], 1.0));
  EXPECT_TRUE(DoubleEqual(converted_data[1].features()[2], 0.5));
  EXPECT_TRUE(DoubleEqual(converted_data[2].features()[2], 0.75));
  EXPECT_TRUE(DoubleEqual(converted_data[3].features()[2], 0.0));
}

TEST_F(FeatureConvertersLearnerTest, FeatureNormalizerLearnerTest) {
  FeatureNormalizerLearner<Object> feature_normalizer_learner;
  feature_normalizer_learner.set_min(-2.0);
  feature_normalizer_learner.set_max(2.0);

  DataSet<Object> train_data(FeatureInfo(3));
  Object object1, object2, object3, object4;
  object1 << 1.0 << 2.0 << 3.0;
  object2 << -2.0 << 1.0 << 1.0;
  object3 << 0.0 << 5.0 << 2.0;
  object4 << 1.0 << 3.0 << -1.0;
  train_data.add(object1);
  train_data.add(object2);
  train_data.add(object3);
  train_data.add(object4);

  feature_normalizer_learner.learn(train_data);
  FeatureConverter::Ptr converter = feature_normalizer_learner.make();

  DataSet<Object> converted_data;
  converter->apply(train_data, &converted_data);

  EXPECT_TRUE(DoubleEqual(converted_data[0].features()[0], 2.0));
  EXPECT_TRUE(DoubleEqual(converted_data[1].features()[0], -2.0));
  EXPECT_TRUE(DoubleEqual(converted_data[2].features()[0], 0.66666666666666));
  EXPECT_TRUE(DoubleEqual(converted_data[3].features()[0], 2.0));

  EXPECT_TRUE(DoubleEqual(converted_data[0].features()[1], -1.0));
  EXPECT_TRUE(DoubleEqual(converted_data[1].features()[1], -2.0));
  EXPECT_TRUE(DoubleEqual(converted_data[2].features()[1], 2.0));
  EXPECT_TRUE(DoubleEqual(converted_data[3].features()[1], 0.0));

  EXPECT_TRUE(DoubleEqual(converted_data[0].features()[2], 2.0));
  EXPECT_TRUE(DoubleEqual(converted_data[1].features()[2], 0.0));
  EXPECT_TRUE(DoubleEqual(converted_data[2].features()[2], 1.0));
  EXPECT_TRUE(DoubleEqual(converted_data[3].features()[2], -2.0));
}
