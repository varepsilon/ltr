// Copyright 2011 Yandex School Practice

#include <gtest/gtest.h>

#include <vector>
#include <limits>

#include "ltr/utility/numerical.h"
#include "ltr/measures/dcg.h"
#include "ltr/measures/ndcg.h"
#include "ltr/measures/average_precision.h"
#include "ltr/measures/reciprocal_rank.h"
#include "ltr/measures/pfound.h"

using ltr::utility::DoubleEqual;
using ltr::ParametersContainer;
using ltr::Object;
using ltr::ObjectList;
using ltr::DCG;
using ltr::YandexDCG;
using ltr::NDCG;
using ltr::YandexNDCG;
using ltr::AveragePrecision;
using ltr::ReciprocalRank;
using ltr::PFound;


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
    o4 << 4;
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
  DCG dcg1;
  dcg1.setIntParameter("NUMBER_OF_OBJECTS_TO_CONSIDER", 1);
  DCG dcg2;
  dcg2.setIntParameter("NUMBER_OF_OBJECTS_TO_CONSIDER", 2);
  DCG dcg3;
  dcg3.setIntParameter("NUMBER_OF_OBJECTS_TO_CONSIDER", 3);

  EXPECT_TRUE(DoubleEqual(dcg1(olist), 15.0));
  EXPECT_TRUE(DoubleEqual(dcg2(olist), 15.6309297535714));
  EXPECT_TRUE(DoubleEqual(dcg3(olist), 17.1309297535714));
  EXPECT_TRUE(DoubleEqual(dcg0(olist), 17.1309297535714));
};

TEST_F(ListwiseMeasuresTest, TestingYandexDCG) {
  YandexDCG dcg0;
  YandexDCG dcg1;
  dcg1.setIntParameter("NUMBER_OF_OBJECTS_TO_CONSIDER", 1);
  YandexDCG dcg2;
  dcg2.setIntParameter("NUMBER_OF_OBJECTS_TO_CONSIDER", 2);
  YandexDCG dcg3;
  dcg3.setIntParameter("NUMBER_OF_OBJECTS_TO_CONSIDER", 3);

  EXPECT_TRUE(DoubleEqual(dcg1(olist), 4.0));
  EXPECT_TRUE(DoubleEqual(dcg2(olist), 4.5));
  EXPECT_TRUE(DoubleEqual(dcg3(olist), 5.273705614469));
  EXPECT_TRUE(DoubleEqual(dcg0(olist), 5.273705614469));
};

TEST_F(ListwiseMeasuresTest, TestingNDCG) {
  NDCG ndcg0;
  NDCG ndcg1;
  ndcg1.setIntParameter("NUMBER_OF_OBJECTS_TO_CONSIDER", 1);
  NDCG ndcg2;
  ndcg2.setIntParameter("NUMBER_OF_OBJECTS_TO_CONSIDER", 2);
  NDCG ndcg3;
  ndcg3.setIntParameter("NUMBER_OF_OBJECTS_TO_CONSIDER", 3);

  EXPECT_TRUE(DoubleEqual(ndcg1(olist), 1.0));
  EXPECT_TRUE(DoubleEqual(ndcg2(olist), 0.92530188545727754));
  EXPECT_TRUE(DoubleEqual(ndcg3(olist), 0.98494436382700368));
  EXPECT_TRUE(DoubleEqual(ndcg0(olist), 0.98494436382700368));
};

TEST_F(ListwiseMeasuresTest, TestingYandexNDCG) {
  YandexNDCG ndcg0;
  YandexNDCG ndcg1;
  ndcg1.setIntParameter("NUMBER_OF_OBJECTS_TO_CONSIDER", 1);
  YandexNDCG ndcg2;
  ndcg2.setIntParameter("NUMBER_OF_OBJECTS_TO_CONSIDER", 2);
  YandexNDCG ndcg3;
  ndcg3.setIntParameter("NUMBER_OF_OBJECTS_TO_CONSIDER", 3);

  EXPECT_TRUE(DoubleEqual(ndcg1(olist), 1.0));
  EXPECT_TRUE(DoubleEqual(ndcg2(olist), 0.9));
  EXPECT_TRUE(DoubleEqual(ndcg3(olist), 0.97899567765921));
  EXPECT_TRUE(DoubleEqual(ndcg0(olist), 0.97899567765921));
};

TEST_F(ListwiseMeasuresTest, TestingAveragePrecision) {
  ParametersContainer param;
  param.setDouble("SCORE_FOR_RELEVANT", 2.0);
  AveragePrecision ap(param);
  EXPECT_TRUE(DoubleEqual(ap(olist), 0.833333333333333333)) << ap(olist);
  EXPECT_TRUE(DoubleEqual(ap(olist2), 0.5)) << ap(olist2);

  param.setDouble("SCORE_FOR_RELEVANT", 1.0);
  AveragePrecision ap1(param);
  EXPECT_TRUE(DoubleEqual(ap1(olist), 1.0)) << ap1(olist);
  EXPECT_TRUE(DoubleEqual(ap1(olist2), 1.0)) << ap1(olist2);

  ap.setDoubleParameter("SCORE_FOR_RELEVANT", 4.0);
  EXPECT_TRUE(DoubleEqual(ap(olist), 1.0)) << ap(olist);
  EXPECT_TRUE(DoubleEqual(ap(olist2), 0.5)) << ap(olist2);

  param.setDouble("SCORE_FOR_RELEVANT", 5.0);
  AveragePrecision ap2(param);
  EXPECT_ANY_THROW(ap2(olist));
}

TEST_F(ListwiseMeasuresTest, TestingReciprocalRank) {
  ParametersContainer param;
  param.setDouble("SCORE_FOR_RELEVANT", 2.0);
  ReciprocalRank rr(param);
  EXPECT_TRUE(DoubleEqual(rr(olist), 1.0)) << rr(olist);
  EXPECT_TRUE(DoubleEqual(rr(olist2), 0.5)) << rr(olist2);

  param.setDouble("SCORE_FOR_RELEVANT", 1.0);
  ReciprocalRank rr1(param);
  EXPECT_TRUE(DoubleEqual(rr1(olist), 1.0)) << rr1(olist);
  EXPECT_TRUE(DoubleEqual(rr1(olist2), 1.0)) << rr1(olist2);

  param.setDouble("SCORE_FOR_RELEVANT", 5.0);
  ReciprocalRank rr2(param);
  EXPECT_ANY_THROW(rr2(olist));
}

TEST_F(ListwiseMeasuresTest, TestingPFoundRank) {
  PFound pf;
  EXPECT_TRUE(DoubleEqual(pf(olist), 0.88024)) << pf(olist);
  EXPECT_TRUE(DoubleEqual(pf(olist2), 0.7985632)) << pf(olist2);

  ParametersContainer param;
  param.setInt("NUMBER_OF_OBJECTS_TO_CONSIDER", 2);
  PFound pf2(param);
  EXPECT_TRUE(DoubleEqual(pf2(olist), 0.834)) << pf2(olist);
  EXPECT_TRUE(DoubleEqual(pf2(olist2), 0.744)) << pf2(olist2);

  pf.setIntParameter("NUMBER_OF_OBJECTS_TO_CONSIDER", 2);
  EXPECT_TRUE(DoubleEqual(pf(olist), pf2(olist)));
  EXPECT_TRUE(DoubleEqual(pf(olist2), pf2(olist2)));

  pf.setIntParameter("NUMBER_OF_OBJECTS_TO_CONSIDER", 5);
  pf.setDoubleParameter("MAX_LABEL", 6.0);
  EXPECT_TRUE(DoubleEqual(pf(olist), 0.78078703703703)) << pf(olist);
  EXPECT_TRUE(DoubleEqual(pf(olist2), 0.71972415123456)) << pf(olist2);

  pf.setIntParameter("NUMBER_OF_OBJECTS_TO_CONSIDER", 0);
  pf.setDoubleParameter("P_BREAK", 0.23);
  EXPECT_TRUE(DoubleEqual(pf(olist), 0.76434259259259)) << pf(olist);
  EXPECT_TRUE(DoubleEqual(pf(olist2), 0.65711983024691)) << pf(olist2);
}

TEST_F(ListwiseMeasuresTest, TestingPFoundRankExceptions) {
  ParametersContainer param;
  param.setDouble("P_BREAK", -0.1);
  EXPECT_ANY_THROW(PFound pf(param));
  param.setDouble("P_BREAK", 2.1);
  EXPECT_ANY_THROW(PFound pf2(param));
  param.setDouble("P_BREAK", 0.1);
  param.setInt("NUMBER_OF_OBJECTS_TO_CONSIDER", -2);
  EXPECT_ANY_THROW(PFound pf3(param));
  param.setInt("NUMBER_OF_OBJECTS_TO_CONSIDER", 1);
  param.setDouble("MAX_LABEL", -8.0);
  EXPECT_ANY_THROW(PFound pf4(param));

  PFound pf;
  olist[1].setActualLabel(-1.0);
  EXPECT_ANY_THROW(pf(olist));
  EXPECT_NO_THROW(pf(olist2));
}
