// Copyright 2011 Yandex School Practice

#include <gtest/gtest.h>

#include <vector>
#include <set>

#include "ltr/data/data_set.h"
#include "ltr/data/object.h"
#include "ltr/data/feature_info.h"

#include "ltr/feature_converters/feature_converter.h"
#include "ltr/feature_converters/utility/utility.h"
#include "ltr/feature_converters/fake_feature_converter.h"
#include "ltr/feature_converters/fake_feature_converter_learner.h"
#include "ltr/feature_converters/feature_subset_chooser.h"
#include "ltr/feature_converters/feature_subset_chooser_learner.h"
#include "ltr/feature_converters/RSM_feature_converter_learner.h"

using std::set;
using std::vector;
using ltr::Object;
using ltr::DataSet;
using ltr::FeatureInfo;
using ltr::FeatureConverter;
using ltr::FakeFeatureConverter;
using ltr::FakeFeatureConverterLearner;
using ltr::FeatureSubsetChooser;
using ltr::FeatureSubsetChooserLearner;
using ltr::RSMFeatureConverterLearner;

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
  if (first.featureInfo() != second.featureInfo()) {
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
  FeatureConverter::Ptr ffc(new FakeFeatureConverter);
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

TEST_F(FeatureConvertersManualTest, FeatureSubsetChooserTest) {
  vector<int> indices;
  indices.push_back(3);
  indices.push_back(7);
  indices.push_back(4);
  FeatureSubsetChooser::Ptr conv(new FeatureSubsetChooser(indices));

  EXPECT_EQ(3, conv->getChoosedFeaturesCount());
  EXPECT_EQ(indices, conv->getChoosedFeaturesIndices());

  DataSet<Object> conv_data;
  ltr::utility::ApplyFeatureConverter(conv, data, &conv_data);

  EXPECT_EQ(3, conv_data.featureInfo().getFeatureCount());
  for (int i = 0; i < indices.size(); ++i) {
    EXPECT_EQ(indices[i], conv_data[0].features()[i]);
  }

  indices.push_back(1);
  conv->setChoosedFeaturesIndices(indices);
  ltr::utility::ApplyFeatureConverter(conv, data, &conv_data);

  EXPECT_EQ(4, conv_data.featureInfo().getFeatureCount());
  for (int i = 0; i < indices.size(); ++i) {
    EXPECT_EQ(indices[i], conv_data[0].features()[i]);
  }

  indices.push_back(103);
  conv->setChoosedFeaturesIndices(indices);
  EXPECT_ANY_THROW(ltr::utility::ApplyFeatureConverter(conv, data, &conv_data));
}

TEST_F(FeatureConvertersManualTest, FeatureSubsetChooserLearnerTest) {
  FeatureSubsetChooserLearner<Object> conv_learner;
  conv_learner.learn(data);
  FeatureConverter::Ptr conv = conv_learner.make();

  DataSet<Object> conv_data;
  ltr::utility::ApplyFeatureConverter(conv, data, &conv_data);

  EXPECT_TRUE(AreEqual(data, conv_data));

  vector<int> unequal;
  unequal.push_back(3);
  unequal.push_back(5);
  unequal.push_back(3);
  EXPECT_ANY_THROW(conv_learner.setListParameter("INDICES", unequal));

  vector<int> indices;
  indices.push_back(3);
  indices.push_back(5);

  conv_learner.setListParameter("INDICES", indices);
  conv_learner.learn(data);
  FeatureConverter::Ptr conv2 = conv_learner.make();

  ltr::utility::ApplyFeatureConverter(conv2, data, &conv_data);
  EXPECT_EQ(indices.size(), conv_data.featureInfo().getFeatureCount());
  for (int i = 0; i < indices.size(); ++i) {
    EXPECT_EQ(indices[i], conv_data[0].features()[i]);
  }
}

TEST_F(FeatureConvertersManualTest, RSMFeatureConverterLearnerTest) {
  RSMFeatureConverterLearner<Object> conv_learner;
  conv_learner.learn(data);
  FeatureConverter::Ptr conv = conv_learner.make();

  DataSet<Object> conv_data;
  ltr::utility::ApplyFeatureConverter(conv, data, &conv_data);

  EXPECT_EQ(4, conv_data.featureInfo().getFeatureCount());
  set<int> used_features;
  for (int i = 0; i < conv_data.featureInfo().getFeatureCount(); ++i) {
    EXPECT_GT(feature_size, conv_data[0].features()[i]);
    EXPECT_LE(0, conv_data[0].features()[i]);

    int current_feature = conv_data[0].features()[i];
    if (used_features.find(current_feature) == used_features.end()) {
      used_features.insert(current_feature);
    } else {
      ADD_FAILURE();
    }
  }

  conv_learner.setDoubleParameter("SELECTED_PART", 0.8);
  conv_learner.learn(data);
  FeatureConverter::Ptr conv2 = conv_learner.make();
  ltr::utility::ApplyFeatureConverter(conv2, data, &conv_data);

  EXPECT_EQ(9, conv_data.featureInfo().getFeatureCount());
  used_features.clear();
  for (int i = 0; i < conv_data.featureInfo().getFeatureCount(); ++i) {
    EXPECT_GT(feature_size, conv_data[0].features()[i]);
    EXPECT_LE(0, conv_data[0].features()[i]);

    int current_feature = conv_data[0].features()[i];
    if (used_features.find(current_feature) == used_features.end()) {
      used_features.insert(current_feature);
    } else {
      ADD_FAILURE();
    }
  }

  EXPECT_ANY_THROW(conv_learner.setDoubleParameter("SELECTED_PART", 0.0));
  conv_learner.setDoubleParameter("SELECTED_PART", 1e-8);
  conv_learner.learn(data);
  FeatureConverter::Ptr conv3 = conv_learner.make();
  ltr::utility::ApplyFeatureConverter(conv3, data, &conv_data);
  EXPECT_EQ(1, conv_data.featureInfo().getFeatureCount());
}
