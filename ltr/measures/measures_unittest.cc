// Copyright 2011 Yandex School Practice

#include <gtest/gtest.h>

#include <boost/filesystem.hpp>

#include <vector>
#include <limits>

#include "ltr/measures/dcg.h"
#include "ltr/measures/ndcg.h"
#include "ltr/utility/numerical.h"

// The fixture for testing (contains data for tests).
class MeasuresTest : public ::testing::Test {
  protected:
  virtual void SetUp() {
    // Code here will be called immediately after the constructor (right
    // before each test).
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test (right
    // before the destructor).
  }
  // objects, used in tests
  protected:
};

// tests.
TEST_F(MeasuresTest, TestingDCG) {
  ltr::DCG dcg0;
  ltr::DCG dcg1(1);
  ltr::DCG dcg2(2);
  ltr::DCG dcg3(3);

  ltr::Object o1;
  o1 << 1;
  o1.setActualLabel(1);
  o1.setPredictedLabel(2);

  ltr::Object o2;
  o2 << 2;
  o2.setActualLabel(2);
  o2.setPredictedLabel(2);


  ltr::Object o3;
  o3 << 3;
  o3.setActualLabel(4);
  o3.setPredictedLabel(3);

  ltr::ObjectList olist;
  olist << o1;
  olist << o2;
  olist << o3;

  EXPECT_TRUE(ltr::utility::DoubleEqual(dcg1(olist), 21.64042561333445));
  EXPECT_TRUE(ltr::utility::DoubleEqual(dcg2(olist), 22.550664839961289));
  EXPECT_TRUE(ltr::utility::DoubleEqual(dcg3(olist), 24.714707401294735));
  EXPECT_TRUE(ltr::utility::DoubleEqual(dcg0(olist), 24.714707401294735));
};

TEST_F(MeasuresTest, TestingNDCG) {
  ltr::NDCG ndcg0;
  ltr::NDCG ndcg1(1);
  ltr::NDCG ndcg2(2);
  ltr::NDCG ndcg3(3);

  ltr::Object o1;
  o1 << 1;
  o1.setActualLabel(1);
  o1.setPredictedLabel(2);

  ltr::Object o2;
  o2 << 2;
  o2.setActualLabel(2);
  o2.setPredictedLabel(2);


  ltr::Object o3;
  o3 << 3;
  o3.setActualLabel(4);
  o3.setPredictedLabel(3);

  ltr::ObjectList olist;
  olist << o1;
  olist << o2;
  olist << o3;

  EXPECT_TRUE(ltr::utility::DoubleEqual(ndcg1(olist), 1.0));
  EXPECT_TRUE(ltr::utility::DoubleEqual(ndcg2(olist), 0.92530188545727754));
  EXPECT_TRUE(ltr::utility::DoubleEqual(ndcg3(olist), 0.98494436382700368));
  EXPECT_TRUE(ltr::utility::DoubleEqual(ndcg0(olist), 0.98494436382700368));
};

