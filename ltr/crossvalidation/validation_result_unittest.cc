// Copyright 2011 Yandex

#include <gtest/gtest.h>

#include <vector>
#include <string>

#include "ltr/crossvalidation/validation_result.h"
#include "ltr/scorers/fake_scorer.h"

using ltr::cv::ValidationResult;
using ltr::FakeScorer;

using std::vector;
using std::string;


TEST(CrossvalidationTest, ValidationResultTest) {
  vector<string> names;
  names.push_back("Measure1");
  names.push_back("Measure2");
  ValidationResult vr(names);

  EXPECT_EQ(names.at(0), vr.getMeasureNames().at(0));
  EXPECT_EQ(names.at(1), vr.getMeasureNames().at(1));

  FakeScorer::Ptr fscorer1(new FakeScorer());
  string report1 = "Report 1";
  vector<double> measures1;
  measures1.push_back(1);
  measures1.push_back(2);
  vr.addSplitInfo(fscorer1, report1, measures1);

  EXPECT_EQ(1, vr.getSplitCount());

  FakeScorer::Ptr fscorer2(new FakeScorer());
  string report2 = "Report 2";
  vector<double> measures2;
  measures2.push_back(3);
  measures2.push_back(4);
  vr.addSplitInfo(fscorer2, report2, measures2);

  EXPECT_EQ(2, vr.getSplitCount());

  EXPECT_EQ(report2, vr.getReport(1));
  EXPECT_EQ(fscorer1, vr.getScorer(0));
  EXPECT_EQ(measures1[0], vr.getMeasureValues(0).at(0));
  EXPECT_EQ(measures2[1], vr.getMeasureValues(1).at(1));

  measures2.push_back(5);
  EXPECT_ANY_THROW(vr.addSplitInfo(fscorer2, report2, measures2));
};
