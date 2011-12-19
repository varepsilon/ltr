// Copyright 2011 Yandex

#include <gtest/gtest.h>
#include <string>

#include "ltr/interfaces/reporter.h"

using ltr::Reporter;
using std::string;


TEST(ReporterTest, AllInReporterTest) {
  Reporter r1;
  Reporter r2("Hello");
  EXPECT_EQ("Hello", r2.report());
  r1.appendReport("Hello");
  EXPECT_EQ("Hello", r1.report());
  r1.appendReport(", world!");
  EXPECT_EQ("Hello, world!", r1.report());
  r2.setReport("Hello, world!");
  EXPECT_EQ("Hello, world!", r2.report());
  r2.clearReport();
  EXPECT_EQ("", r2.report());
};
