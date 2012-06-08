// Copyright 2011 Yandex School Practice

#include <gtest/gtest.h>

#include <vector>
#include <set>

#include "ltr/data/data_set.h"
#include "ltr/data/object.h"
#include "ltr/data/feature_info.h"
#include "ltr/utility/numerical.h"

#include "ltr/feature_converters/feature_converter.h"
#include "ltr/feature_converters/utility/utility.h"
#include "ltr/feature_converters/fake_feature_converter.h"
#include "ltr/feature_converters/fake_feature_converter_learner.h"
#include "ltr/feature_converters/feature_sampler.h"
#include "ltr/feature_converters/feature_sampler_learner.h"
#include "ltr/feature_converters/feature_random_sampler_learner.h"
#include "ltr/feature_converters/per_feature_linear_converter.h"
#include "ltr/feature_converters/feature_normalizer_learner.h"

using std::set;
using std::vector;
using ltr::Object;
using ltr::DataSet;
using ltr::FeatureInfo;
using ltr::utility::DoubleEqual;
using ltr::FeatureConverter;
using ltr::FakeFeatureConverter;
using ltr::FakeFeatureConverterLearner;
using ltr::FeatureSampler;
using ltr::FeatureSamplerLearner;
using ltr::FeatureRandomSamplerLearner;
using ltr::PerFeatureLinearConverter;
using ltr::FeatureNormalizerLearner;

const int feature_size = 11;

class FeatureConvertersManualTest : public ::testing::Test {
  public:
  FeatureConvertersManualTest()
    : data(FeatureInfo(feature_size)) {}
  protected:
  virtual void SetUp() {
    Object obj;
    for (int i = 0; i < feature_size; ++i) {
      obj << i;
    }
    data.add(obj);
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
  if (first.feature_info() != second.feature_info()) {
    return false;
  }
  for (int i = 0; i < first.size(); ++i) {
    if (first[i].features() != second[i].features()) {
      return false;
    }
  }
  return true;
}

TEST_F(FeatureConvertersManualTest, FakeFeatureConverterTest) {
  FakeFeatureConverter::Ptr ffc
    (new FakeFeatureConverter(data.feature_info()));
  DataSet<Object> conv_data;

  ltr::utility::ApplyFeatureConverter(ffc, data, &conv_data);
  EXPECT_TRUE(AreEqual(data, conv_data));
}

TEST_F(FeatureConvertersManualTest, FakeFeatureConverterLearnerTest) {
  FakeFeatureConverterLearner<Object> conv_learner;
  conv_learner.learn(data);
  FeatureConverter::Ptr conv = conv_learner.make();

  DataSet<Object> conv_data;

  ltr::utility::ApplyFeatureConverter(conv, data, &conv_data);
  EXPECT_TRUE(AreEqual(data, conv_data));
}

TEST_F(FeatureConvertersManualTest, FeatureSamplerTest) {
  vector<int> indices;
  indices.push_back(3);
  indices.push_back(7);
  indices.push_back(4);
  FeatureSampler::Ptr conv(new FeatureSampler(indices));
  conv->set_input_feature_info(data.feature_info());

  EXPECT_EQ(3, conv->getChoosedFeaturesCount());
  EXPECT_EQ(indices, conv->getChoosedFeaturesIndices());

  DataSet<Object> conv_data;
  ltr::utility::ApplyFeatureConverter(conv, data, &conv_data);

  EXPECT_EQ(3, conv_data.feature_info().get_feature_count());
  for (int i = 0; i < indices.size(); ++i) {
    EXPECT_EQ(indices[i], conv_data[0].features()[i]);
  }

  indices.push_back(1);
  conv->setChoosedFeaturesIndices(indices);
  ltr::utility::ApplyFeatureConverter(conv, data, &conv_data);

  EXPECT_EQ(4, conv_data.feature_info().get_feature_count());
  for (int i = 0; i < indices.size(); ++i) {
    EXPECT_EQ(indices[i], conv_data[0].features()[i]);
  }

  indices.push_back(103);
  EXPECT_ANY_THROW(conv->setChoosedFeaturesIndices(indices));
}

TEST_F(FeatureConvertersManualTest, FeatureSamplerLearnerTest) {
  FeatureSamplerLearner<Object> conv_learner;
  conv_learner.learn(data);
  FeatureConverter::Ptr conv = conv_learner.make();

  DataSet<Object> conv_data;
  ltr::utility::ApplyFeatureConverter(conv, data, &conv_data);

  EXPECT_TRUE(AreEqual(data, conv_data));

  vector<int> unequal;
  unequal.push_back(3);
  unequal.push_back(5);
  unequal.push_back(3);
  EXPECT_ANY_THROW(conv_learner.setExistingParameter("INDICES", unequal));

  vector<int> indices;
  indices.push_back(3);
  indices.push_back(5);

  conv_learner.setExistingParameter("INDICES", indices);
  conv_learner.learn(data);
  FeatureConverter::Ptr conv2 = conv_learner.make();

  ltr::utility::ApplyFeatureConverter(conv2, data, &conv_data);
  EXPECT_EQ(indices.size(), conv_data.feature_info().get_feature_count());
  for (int i = 0; i < indices.size(); ++i) {
    EXPECT_EQ(indices[i], conv_data[0].features()[i]);
  }
}

TEST_F(FeatureConvertersManualTest, FeatureRandomSamplerLearnerTest) {
  FeatureRandomSamplerLearner<Object> conv_learner;
  conv_learner.learn(data);
  FeatureConverter::Ptr conv = conv_learner.make();

  DataSet<Object> conv_data;
  ltr::utility::ApplyFeatureConverter(conv, data, &conv_data);

  EXPECT_EQ(4, conv_data.feature_info().get_feature_count());
  set<int> used_features;
  for (int i = 0; i < conv_data.feature_info().get_feature_count(); ++i) {
    EXPECT_GT(feature_size, conv_data[0].features()[i]);
    EXPECT_LE(0, conv_data[0].features()[i]);

    int current_feature = conv_data[0].features()[i];
    if (used_features.find(current_feature) == used_features.end()) {
      used_features.insert(current_feature);
    } else {
      ADD_FAILURE();
    }
  }

  conv_learner.setExistingParameter("SELECTED_PART", 0.8);
  conv_learner.learn(data);
  FeatureConverter::Ptr conv2 = conv_learner.make();
  ltr::utility::ApplyFeatureConverter(conv2, data, &conv_data);

  EXPECT_EQ(9, conv_data.feature_info().get_feature_count());
  used_features.clear();
  for (int i = 0; i < conv_data.feature_info().get_feature_count(); ++i) {
    EXPECT_GT(feature_size, conv_data[0].features()[i]);
    EXPECT_LE(0, conv_data[0].features()[i]);

    int current_feature = conv_data[0].features()[i];
    if (used_features.find(current_feature) == used_features.end()) {
      used_features.insert(current_feature);
    } else {
      ADD_FAILURE();
    }
  }

  EXPECT_ANY_THROW(conv_learner.setExistingParameter("SELECTED_PART", 0.0));
  conv_learner.setExistingParameter("SELECTED_PART", 1e-8);
  conv_learner.learn(data);
  FeatureConverter::Ptr conv3 = conv_learner.make();
  ltr::utility::ApplyFeatureConverter(conv3, data, &conv_data);
  EXPECT_EQ(1, conv_data.feature_info().get_feature_count());
}

TEST_F(FeatureConvertersManualTest, PerFeatureLinearConverterTest) {
  PerFeatureLinearConverter::Ptr pf_converter
    (new PerFeatureLinearConverter(data.feature_info().get_feature_count()));
  pf_converter->set_input_feature_info(data.feature_info());

  pf_converter->setShift(0, 1.0);
  pf_converter->setShift(1, 0.0);
  pf_converter->setShift(2, -1.0);
  pf_converter->setShift(3, 0.0);

  pf_converter->setCoefficient(0, 1.0);
  pf_converter->setCoefficient(1, -0.5);
  pf_converter->setCoefficient(2, 2.0);
  pf_converter->setCoefficient(3, 0.0);

  DataSet<Object> conv_data;

  ltr::utility::ApplyFeatureConverter(pf_converter, data, &conv_data);

  EXPECT_TRUE(DoubleEqual(conv_data[0].features()[0], 1.0));
  EXPECT_TRUE(DoubleEqual(conv_data[0].features()[1], -0.5));
  EXPECT_TRUE(DoubleEqual(conv_data[0].features()[2], 3.0));
  EXPECT_TRUE(DoubleEqual(conv_data[0].features()[3], 0.0));
}

TEST_F(FeatureConvertersManualTest, FeatureNormalizerLearnerDefaultTest) {
  FeatureNormalizerLearner<Object> conv_learner;

  DataSet<Object> l_data(FeatureInfo(3));
  Object o1, o2, o3, o4;
  o1 << 1.0 << 2.0 << 3.0;
  o2 << -2.0 << 1.0 << 1.0;
  o3 << 0.0 << 5.0 << 2.0;
  o4 << 1.0 << 3.0 << -1.0;
  l_data.add(o1);
  l_data.add(o2);
  l_data.add(o3);
  l_data.add(o4);

  conv_learner.learn(l_data);
  FeatureConverter::Ptr conv = conv_learner.make();

  DataSet<Object> conv_data;
  ltr::utility::ApplyFeatureConverter(conv, l_data, &conv_data);

  EXPECT_TRUE(DoubleEqual(conv_data[0].features()[0], 1.0));
  EXPECT_TRUE(DoubleEqual(conv_data[1].features()[0], 0.0));
  EXPECT_TRUE(DoubleEqual(conv_data[2].features()[0], 0.66666666666666));
  EXPECT_TRUE(DoubleEqual(conv_data[3].features()[0], 1.0));

  EXPECT_TRUE(DoubleEqual(conv_data[0].features()[1], 0.25));
  EXPECT_TRUE(DoubleEqual(conv_data[1].features()[1], 0.0));
  EXPECT_TRUE(DoubleEqual(conv_data[2].features()[1], 1.0));
  EXPECT_TRUE(DoubleEqual(conv_data[3].features()[1], 0.5));

  EXPECT_TRUE(DoubleEqual(conv_data[0].features()[2], 1.0));
  EXPECT_TRUE(DoubleEqual(conv_data[1].features()[2], 0.5));
  EXPECT_TRUE(DoubleEqual(conv_data[2].features()[2], 0.75));
  EXPECT_TRUE(DoubleEqual(conv_data[3].features()[2], 0.0));
}

TEST_F(FeatureConvertersManualTest, FeatureNormalizerLearnerTest) {
  FeatureNormalizerLearner<Object> conv_learner;
  conv_learner.setExistingParameter("NormalizationIntervalBegin", -2.0);
  conv_learner.setExistingParameter("NormalizationIntervalEnd", 2.0);

  DataSet<Object> l_data(FeatureInfo(3));
  Object o1, o2, o3, o4;
  o1 << 1.0 << 2.0 << 3.0;
  o2 << -2.0 << 1.0 << 1.0;
  o3 << 0.0 << 5.0 << 2.0;
  o4 << 1.0 << 3.0 << -1.0;
  l_data.add(o1);
  l_data.add(o2);
  l_data.add(o3);
  l_data.add(o4);

  conv_learner.learn(l_data);
  FeatureConverter::Ptr conv = conv_learner.make();

  DataSet<Object> conv_data;
  ltr::utility::ApplyFeatureConverter(conv, l_data, &conv_data);

  EXPECT_TRUE(DoubleEqual(conv_data[0].features()[0], 2.0));
  EXPECT_TRUE(DoubleEqual(conv_data[1].features()[0], -2.0));
  EXPECT_TRUE(DoubleEqual(conv_data[2].features()[0], 0.66666666666666));
  EXPECT_TRUE(DoubleEqual(conv_data[3].features()[0], 2.0));

  EXPECT_TRUE(DoubleEqual(conv_data[0].features()[1], -1.0));
  EXPECT_TRUE(DoubleEqual(conv_data[1].features()[1], -2.0));
  EXPECT_TRUE(DoubleEqual(conv_data[2].features()[1], 2.0));
  EXPECT_TRUE(DoubleEqual(conv_data[3].features()[1], 0.0));

  EXPECT_TRUE(DoubleEqual(conv_data[0].features()[2], 2.0));
  EXPECT_TRUE(DoubleEqual(conv_data[1].features()[2], 0.0));
  EXPECT_TRUE(DoubleEqual(conv_data[2].features()[2], 1.0));
  EXPECT_TRUE(DoubleEqual(conv_data[3].features()[2], -2.0));
}
