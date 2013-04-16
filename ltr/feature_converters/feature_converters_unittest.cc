// Copyright 2012 Yandex

#include <gtest/gtest.h>

#include <map>
#include <string>

#include "contrib/include_Eigen.h"

#include "ltr/data/object.h"
#include "ltr/data/data_set.h"
#include "ltr/data/feature_info.h"
#include "ltr/utility/indices.h"
#include "ltr/utility/numerical.h"

#include "ltr/feature_converters/feature_converter.h"
#include "ltr/feature_converters/fake_feature_converter.h"
#include "ltr/feature_converters/feature_sampler.h"
#include "ltr/feature_converters/linear_converter.h"
#include "ltr/feature_converters/nan_to_neutral_converter.h"
#include "ltr/feature_converters/nominal_to_bool_converter.h"
#include "ltr/feature_converters/per_feature_linear_converter.h"
#include "ltr/feature_converters/remove_nominal_converter.h"

using ltr::Object;
using ltr::DataSet;
using ltr::FeatureInfo;
using ltr::NOMINAL;
using ltr::NUMERIC;
using ltr::utility::DoubleEqual;
using ltr::utility::Indices;

using ltr::FeatureConverter;
using ltr::FakeFeatureConverter;
using ltr::FeatureSampler;
using ltr::LinearConverter;
using ltr::NanToNeutralConverter;
using ltr::NominalToBoolConverter;
using ltr::PerFeatureLinearConverter;
using ltr::RemoveNominalConverter;

using Eigen::MatrixXd;
using Eigen::VectorXd;

TEST(FeatureConvertersTest, FakeFeatureConverterTest) {
  Object object;
  object << 0.43532 << -1.435 << 2.435435 << 0.546;

  DataSet<Object> data, converted_data;
  data.add(object);

  FakeFeatureConverter::Ptr fake_feature_converter
    (new FakeFeatureConverter(data.feature_info()));

  fake_feature_converter->apply(data, &converted_data);

  EXPECT_EQ(data[0], converted_data[0]);
}

TEST(FeatureConvertersTest, FeatureSamplerTest) {
  Object object;
  object << 0.43532 << -1.435 << 2.435435 << 0.546 << 12.23143;

  Indices indices;
  indices.push_back(2);
  indices.push_back(4);
  indices.push_back(3);

  DataSet<Object> data, converted_data;
  data.add(object);

  FeatureSampler::Ptr converter(new FeatureSampler(indices));
  converter->set_input_feature_info(data.feature_info());

  EXPECT_EQ(indices, converter->indices());

  converter->apply(data, &converted_data);

  EXPECT_EQ(indices.size(), converted_data[0].feature_count());
  EXPECT_EQ(object[2], converted_data[0][0]);
  EXPECT_EQ(object[4], converted_data[0][1]);
  EXPECT_EQ(object[3], converted_data[0][2]);

  indices.push_back(1);
  converter->set_indices(indices);
  converter->apply(data, &converted_data);

  EXPECT_EQ(indices.size(), converted_data[0].feature_count());
  EXPECT_EQ(object[2], converted_data[0][0]);
  EXPECT_EQ(object[4], converted_data[0][1]);
  EXPECT_EQ(object[3], converted_data[0][2]);
  EXPECT_EQ(object[1], converted_data[0][3]);

  indices.push_back(103);
  EXPECT_ANY_THROW(converter->set_indices(indices));
}

TEST(FeatureConvertersTest, PerFeatureLinearConverterTest) {
  Object object;
  object << 0 << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9 << 10;

  DataSet<Object> data, converted_data;
  data.add(object);

  PerFeatureLinearConverter::Ptr per_feature_converter
    (new PerFeatureLinearConverter(object.feature_count()));

  per_feature_converter->set_input_feature_info(data.feature_info());

  per_feature_converter->set_shift(0, 1.0);
  per_feature_converter->set_shift(1, 0.0);
  per_feature_converter->set_shift(2, -1.0);
  per_feature_converter->set_shift(3, 0.0);

  per_feature_converter->set_factor(0, 1.0);
  per_feature_converter->set_factor(1, -0.5);
  per_feature_converter->set_factor(2, 2.0);
  per_feature_converter->set_factor(3, 0.0);

  per_feature_converter->apply(data, &converted_data);

  EXPECT_TRUE(DoubleEqual(converted_data[0][0], 1.0));
  EXPECT_TRUE(DoubleEqual(converted_data[0][1], -0.5));
  EXPECT_TRUE(DoubleEqual(converted_data[0][2], 3.0));
  EXPECT_TRUE(DoubleEqual(converted_data[0][3], 0.0));
}

TEST(FeatureConvertersTest, LinearConverterTest) {
  Object object1, object2;
  object1 << 1.0 << 2.0 << 3.0;
  object2 << -1.0 << 1.0 << 0.0;
  DataSet<Object> data, converted_data;
  data.add(object1);
  data.add(object2);

  MatrixXd factor(2, 3);
  factor << 1.0,  0.0,  2.0,
            -1.0, 10.0, 5.0;

  VectorXd shift(2);
  shift << 0.0, 1.0;

  LinearConverter::Ptr linear_converter
    (new LinearConverter(object1.feature_count()));
  linear_converter->set_input_feature_info(data.feature_info());
  linear_converter->set_factor(factor);
  linear_converter->set_shift(shift);
  linear_converter->apply(data, &converted_data);
  EXPECT_TRUE(DoubleEqual(converted_data[0][0], 7.0));
  EXPECT_TRUE(DoubleEqual(converted_data[0][1], 35.0));
  EXPECT_TRUE(DoubleEqual(converted_data[1][0], -1.0));
  EXPECT_TRUE(DoubleEqual(converted_data[1][1], 12.0));
}
TEST(FeatureConvertersTest, NanToNeutralConverterTest) {
  Object nan_features_object, object;
  nan_features_object << numeric_limits<double>::quiet_NaN();
  nan_features_object << numeric_limits<double>::quiet_NaN();
  nan_features_object << numeric_limits<double>::quiet_NaN();
  object << 1 << 2 << 3;

  DataSet<Object> data, converted_data;
  data.add(nan_features_object);

  NanToNeutralConverter::Ptr nan_to_neutral_converter
    (new NanToNeutralConverter(data.feature_info()));

  nan_to_neutral_converter->set_neutral_object(object);
  nan_to_neutral_converter->apply(data, &converted_data);

  EXPECT_EQ(converted_data[0], object);
}

TEST(FeatureConvertersTest, NominalToBoolConverterTest) {
  FeatureInfo feature_info;
  map <int, std::string> nominal_feature_values1, nominal_feature_values2;
  nominal_feature_values1[1] = "red";
  nominal_feature_values2[2] = "green";
  feature_info.addFeature(NOMINAL,
                          ltr::NominalFeatureValues(nominal_feature_values1));
  feature_info.addFeature(NOMINAL,
                          ltr::NominalFeatureValues(nominal_feature_values2));

  NominalToBoolConverter::Ptr nominal_to_bool_converter
    (new NominalToBoolConverter(feature_info));

  Object object;
  object << 1 << 1;

  DataSet<Object> data(feature_info), converted_data;
  data.add(object);

  nominal_to_bool_converter->apply(data, &converted_data);
  EXPECT_EQ(2, converted_data[0].feature_count());
  EXPECT_EQ(ltr::BOOLEAN, converted_data.feature_info().getFeatureType(0));
  EXPECT_EQ(1, converted_data[0][0]);
  EXPECT_EQ(ltr::BOOLEAN, converted_data.feature_info().getFeatureType(1));
  EXPECT_EQ(0, converted_data[0][1]);

  feature_info.addFeature(NUMERIC);
  feature_info.addFeature(NOMINAL);

  object << 10 << 1;
  DataSet<Object> data2;
  data2 << object;

  NominalToBoolConverter::Ptr nominal_to_bool_converter2
    (new NominalToBoolConverter(feature_info));

  nominal_to_bool_converter2->apply(data2, &converted_data);

  EXPECT_EQ(4, converted_data[0].feature_count());
  EXPECT_EQ(NUMERIC, converted_data.feature_info().getFeatureType(0));
  EXPECT_EQ(converted_data[0][0], 10);
  EXPECT_EQ(ltr::BOOLEAN, converted_data.feature_info().getFeatureType(1));
  EXPECT_EQ(1, converted_data[0][1]);
  EXPECT_EQ(ltr::BOOLEAN, converted_data.feature_info().getFeatureType(2));
  EXPECT_EQ(0, converted_data[0][2]);
  EXPECT_EQ(ltr::BOOLEAN, converted_data.feature_info().getFeatureType(3));
}

TEST(FeatureConvertersTest, RemoveNominalConverterTest) {
  FeatureInfo feature_info;
  feature_info.addFeature(NOMINAL);

  RemoveNominalConverter::Ptr remove_nominal_converter
    (new RemoveNominalConverter(feature_info));

  DataSet<Object> data(feature_info), converted_data;

  Object non_empty_object;
  non_empty_object << 1;
  data.add(non_empty_object);

  remove_nominal_converter->apply(data, &converted_data);

  EXPECT_TRUE(converted_data[0].feature_count() == 0);

  feature_info.addFeature(NUMERIC);
  feature_info.addFeature(NOMINAL);
  Object object;
  object << 1 << 2 << 3;

  DataSet<Object> data2(feature_info);
  data2.add(object);

  RemoveNominalConverter::Ptr remove_nominal_converter2
    (new RemoveNominalConverter(feature_info));

  remove_nominal_converter2->apply(data2, &converted_data);

  EXPECT_EQ(1, converted_data[0].feature_count());
  EXPECT_EQ(2, converted_data[0][0]);
}
