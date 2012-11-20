// Copyright 2012 Yandex

#include "gtest/gtest.h"

#include "ltr/utility/boost/lexical_cast.h"
#include <string>

using ltr::utility::lexical_cast;
using std::string;

TEST(LexicalCastTest, TestingFromStringConversion) {
  EXPECT_EQ(lexical_cast<int>("42"), 42);
  EXPECT_EQ(lexical_cast<double>("42"), 42.0);
  EXPECT_EQ(lexical_cast<double>("42.0"), 42.0);
  EXPECT_EQ(lexical_cast<double>("42.0000000001"), 42.0000000001);
  EXPECT_EQ(lexical_cast<bool>("1"), true);
  EXPECT_EQ(lexical_cast<string>("42.0"), "42.0");

  EXPECT_ANY_THROW(lexical_cast<int>("42a"));
  EXPECT_ANY_THROW(lexical_cast<int>("42 a"));
}

TEST(LexicalCastTest, TestingToStringConversion) {
  EXPECT_EQ(lexical_cast<string>(42), "42");
  EXPECT_EQ(lexical_cast<string>(42.0), "42.000000000000");
  EXPECT_EQ(lexical_cast<string>(42.0000000001), "42.000000000100");
  EXPECT_EQ(lexical_cast<string>(true), "1");
  EXPECT_EQ(lexical_cast<string>("42"), "42");
}
