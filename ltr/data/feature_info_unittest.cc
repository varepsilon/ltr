// Copyright 2012 Yandex School Practice

#include <stdlib.h>
#include <string>

#include "gtest/gtest.h"

#include "ltr/data/feature_info.h"

using ltr::FeatureInfo;
using ltr::NominalFeatureValues;

TEST(FeaturesInfoTest, TestingBasicOperations) {
  FeatureInfo info1(5, ltr::NUMERIC);
  for (size_t i = 0; i < 5; ++i)
    EXPECT_EQ(info1.getFeatureType(i), ltr::NUMERIC);

  NominalFeatureValues values;
  values[235] = "fdsf";
  values[65342] = "645333gf";
  info1.addFeature(ltr::NOMINAL, values);

  EXPECT_EQ(info1.getFeatureType(5), ltr::NOMINAL);
  EXPECT_EQ(info1.getFeatureValues(5), values);

  FeatureInfo info2;
  info2.resize(5, ltr::NUMERIC);

  EXPECT_FALSE(info2 == info1);
  info2.addFeature(ltr::NOMINAL, values);

  EXPECT_EQ(info1, info2);
}
