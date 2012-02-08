// Copyright 2011 Yandex School Practice

#include <gtest/gtest.h>

#include <boost/filesystem.hpp>

#include <vector>
#include <limits>

#include "ltr/measures/dcg.h"
#include "ltr/measures/ndcg.h"
#include "ltr/measures/average_precision.h"
#include "ltr/utility/numerical.h"

using ltr::Object;
using ltr::ObjectList;
using ltr::DCG;
using ltr::NDCG;
using ltr::AveragePrecision;
using ltr::utility::DoubleEqual;
using ltr::ParametersContainer;

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

    Object o4;
    o3 << 4;
    o4.setActualLabel(1);
    o4.setPredictedLabel(3);

    olist2 << o1;
    olist2 << o2;
    olist2 << o3;
    olist2 << o4;
  }

  virtual void TearDown() {
  }

  protected:
    ObjectList olist;
    ObjectList olist2;
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

TEST_F(ListwiseMeasuresTest, TestingAveragePrecision) {
  ParametersContainer param;
  param.setDouble("SCORE_FOR_RELEVANT", 2.0);
  AveragePrecision ap0(param);
  EXPECT_TRUE(DoubleEqual(ap0(olist), 0.833333333333333333)) << ap0(olist);
  EXPECT_TRUE(DoubleEqual(ap0(olist2), 0.5)) << ap0(olist2);

  param.setDouble("SCORE_FOR_RELEVANT", 1.0);
  AveragePrecision ap1(param);
  EXPECT_TRUE(DoubleEqual(ap1(olist), 1.0)) << ap1(olist);
  EXPECT_TRUE(DoubleEqual(ap1(olist2), 1.0)) << ap1(olist2);

  param.setDouble("SCORE_FOR_RELEVANT", 4.0);
  AveragePrecision ap2(param);
  EXPECT_TRUE(DoubleEqual(ap2(olist), 1.0)) << ap2(olist);
  EXPECT_TRUE(DoubleEqual(ap2(olist2), 0.5)) << ap2(olist2);

  param.setDouble("SCORE_FOR_RELEVANT", 5.0);
  AveragePrecision ap3(param);
  EXPECT_ANY_THROW(ap3(olist));
}
