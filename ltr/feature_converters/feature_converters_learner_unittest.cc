// Copyright 2012 Yandex

#include <gtest/gtest.h>

#include <vector>

#include "ltr/data/data_set.h"
#include "ltr/data/object.h"
#include "ltr/data/feature_info.h"
#include "ltr/utility/indices.h"

#include "ltr/feature_converters/feature_converter.h"
#include "ltr/feature_converters/feature_converter_learner.h"
#include "ltr/feature_converters/fake_feature_converter_learner.h"
#include "ltr/feature_converters/feature_sampler_learner.h"
#include "ltr/feature_converters/feature_random_sampler_learner.h"
#include "ltr/feature_converters/feature_normalizer_learner.h"
#include "ltr/feature_converters/nan_to_zero_converter.h"
#include "ltr/feature_converters/nominal_to_bool_converter.h"
#include "ltr/feature_converters/remove_nominal_converter.h"

using std::vector;

using ltr::Object;
using ltr::DataSet;
using ltr::FeatureInfo;
using ltr::utility::DoubleEqual;
using ltr::utility::IndicesPtr;
using ltr::utility::Indices;

using ltr::FeatureConverter;
using ltr::FeatureConverterLearner;
using ltr::BaseFeatureConverterLearner;
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
  vector<FeatureConverterLearner<Object>::Ptr> feature_converters;

  feature_converters.push_back(
    static_cast<FakeFeatureConverterLearner<Object>::Ptr>(
      new FakeFeatureConverterLearner<Object>));
  feature_converters.push_back(
    static_cast<FeatureNormalizerLearner<Object>::Ptr>(
      new FeatureNormalizerLearner<Object>));
  feature_converters.push_back(
    static_cast<FeatureRandomSamplerLearner<Object>::Ptr>(
      new FeatureRandomSamplerLearner<Object>));
  feature_converters.push_back(
    static_cast<FeatureSamplerLearner<Object>::Ptr>(
      new FeatureSamplerLearner<Object>));
  feature_converters.push_back(
    static_cast<NanToZeroConverterLearner<Object>::Ptr>(
      new NanToZeroConverterLearner<Object>));
  feature_converters.push_back(
    static_cast<NominalToBoolConverterLearner<Object>::Ptr>(
      new NominalToBoolConverterLearner<Object>));
  feature_converters.push_back(
    static_cast<RemoveNominalConverterLearner<Object>::Ptr>(
      new RemoveNominalConverterLearner<Object>));

  for (int feature_converters_index = 0;
       feature_converters_index < feature_converters.size();
       ++feature_converters_index) {
    ASSERT_NO_THROW(feature_converters[feature_converters_index]->learn(data));
  }

  DataSet<Object> converted_data;

  for (int feature_converters_index = 0;
       feature_converters_index < feature_converters.size();
       ++feature_converters_index) {
    FeatureConverter::Ptr feature_converter =
      feature_converters[feature_converters_index]->make();
    ASSERT_NO_THROW(feature_converter->apply(data, &converted_data));
  }
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
    EXPECT_EQ((*indices)[index], converted_data[0][index]);
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
    EXPECT_GT(features_count, converted_data[0][feature_index]);
    EXPECT_LE(0, converted_data[0][feature_index]);

    int current_feature = converted_data[0][feature_index];
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
    EXPECT_GT(features_count, converted_data[0][feature_index]);
    EXPECT_LE(0, converted_data[0][feature_index]);

    int current_feature = converted_data[0][feature_index];
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

TEST_F(FeatureConvertersLearnerTest, FeatureNormalizerLearnerTest) {
  FeatureNormalizerLearner<Object> bad_feature_normalizer_learner;
  bad_feature_normalizer_learner.set_min(2.0);
  bad_feature_normalizer_learner.set_max(-2.0);
  ASSERT_ANY_THROW(bad_feature_normalizer_learner.checkParameters());

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
  ASSERT_NO_THROW(feature_normalizer_learner.checkParameters());

  DataSet<Object> converted_data;
  converter->apply(train_data, &converted_data);

  EXPECT_TRUE(DoubleEqual(converted_data[0][0], 2.0));
  EXPECT_TRUE(DoubleEqual(converted_data[1][0], -2.0));
  EXPECT_TRUE(DoubleEqual(converted_data[2][0], 0.66666666666666));
  EXPECT_TRUE(DoubleEqual(converted_data[3][0], 2.0));

  EXPECT_TRUE(DoubleEqual(converted_data[0][1], -1.0));
  EXPECT_TRUE(DoubleEqual(converted_data[1][1], -2.0));
  EXPECT_TRUE(DoubleEqual(converted_data[2][1], 2.0));
  EXPECT_TRUE(DoubleEqual(converted_data[3][1], 0.0));

  EXPECT_TRUE(DoubleEqual(converted_data[0][2], 2.0));
  EXPECT_TRUE(DoubleEqual(converted_data[1][2], 0.0));
  EXPECT_TRUE(DoubleEqual(converted_data[2][2], 1.0));
  EXPECT_TRUE(DoubleEqual(converted_data[3][2], -2.0));
}
