// Copyright 2011 Yandex School Practice

#include <gtest/gtest.h>

#include <boost/filesystem.hpp>

#include <vector>
#include <limits>

#include "ltr/measures/dcg.h"
#include "ltr/measures/ndcg.h"
#include "ltr/utility/numerical.h"

using ltr::Object;
using ltr::ObjectList;
using ltr::DCG;
using ltr::NDCG;
using ltr::utility::DoubleEqual;

class ListwiseMeasuresTest : public ::testing::Test {
  protected:
  virtual void SetUp() {
    Object o1;
    o1 << 1;
    o1.setActualLabel(1);
    o1.setPredictedLabel(2);

    Object o2;
    o2 << 2;
    o2.setActualLabel(2);
    o2.setPredictedLabel(2);

    Object o3;
    o3 << 3;
    o3.setActualLabel(4);
    o3.setPredictedLabel(3);

    olist << o1;
    olist << o2;
    olist << o3;
  }

  virtual void TearDown() {
  }

  protected:
    ObjectList olist;
};


TEST_F(ListwiseMeasuresTest, TestingDCG) {
  DCG dcg0;
  DCG dcg1(1);
  DCG dcg2(2);
  DCG dcg3(3);

  EXPECT_TRUE(DoubleEqual(dcg1(olist), 21.64042561333445));
  EXPECT_TRUE(DoubleEqual(dcg2(olist), 22.550664839961289));
  EXPECT_TRUE(DoubleEqual(dcg3(olist), 24.714707401294735));
  EXPECT_TRUE(DoubleEqual(dcg0(olist), 24.714707401294735));
};

TEST_F(ListwiseMeasuresTest, TestingNDCG) {
  NDCG ndcg0;
  NDCG ndcg1(1);
  NDCG ndcg2(2);
  NDCG ndcg3(3);

  EXPECT_TRUE(DoubleEqual(ndcg1(olist), 1.0));
  EXPECT_TRUE(DoubleEqual(ndcg2(olist), 0.92530188545727754));
  EXPECT_TRUE(DoubleEqual(ndcg3(olist), 0.98494436382700368));
  EXPECT_TRUE(DoubleEqual(ndcg0(olist), 0.98494436382700368));
};

