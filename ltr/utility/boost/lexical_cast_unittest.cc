// Copyright 2012 Yandex
#include <string>

#include "gtest/gtest.h"

#include "ltr/utility/boost/lexical_cast.hpp"
#include "ltr/utility/numerical.h"

using ltr::utility::lexical_cast;
using ltr::utility::DoubleEqual;
using ltr::utility::any_cast;
using std::string;

TEST(LexicalCastTest, TestingFromStringConversion) {
  EXPECT_EQ(lexical_cast<int>(" 42  "), 42);
  EXPECT_TRUE(DoubleEqual(lexical_cast<double>("42"), 42.0));
  EXPECT_TRUE(DoubleEqual(lexical_cast<double>("-42.0"), -42.0));
  EXPECT_TRUE(DoubleEqual(lexical_cast<double>("42.000000001"), 42.000000001));
  EXPECT_TRUE(lexical_cast<bool>("true"));
  EXPECT_TRUE(!lexical_cast<bool>("False"));
  EXPECT_EQ(lexical_cast<string>("Hello!"), "Hello!");

  vector<int> test = lexical_cast<vector<int> >("1, 1000000000 , -42 ");
  EXPECT_EQ(test[0], 1);
  EXPECT_EQ(test[1], 1000000000);
  EXPECT_EQ(test[2], -42);

  EXPECT_ANY_THROW(lexical_cast<int>("42a"));
  EXPECT_ANY_THROW(lexical_cast<int>("42 a"));
}

TEST(LexicalCastTest, TestingToStringConversion) {
  EXPECT_EQ(lexical_cast<string>(42), "42");
  EXPECT_EQ(lexical_cast<string>(42.0), "42.000000000000");
  EXPECT_EQ(lexical_cast<string>(42.0000000001), "42.000000000100");
  EXPECT_EQ(lexical_cast<string>(true), "true");
  EXPECT_EQ(lexical_cast<string>("Hello,world!"), "Hello,world!");
}

TEST(LexicalCastTest, TestingToAnyConversion) {
  EXPECT_EQ(any_cast<int>(lexical_cast<Any>("42")), 42);
  EXPECT_TRUE(any_cast<bool>(lexical_cast<Any>("true")));
  EXPECT_EQ(any_cast<string>(lexical_cast<Any>("Hello!")), "Hello!");

  vector<int> test1;
  test1.push_back(42);
  test1.push_back(24);
  EXPECT_EQ(any_cast<vector<int> >(lexical_cast<Any>("42, 24"))[0], test1[0]);
  EXPECT_EQ(any_cast<vector<int> >(lexical_cast<Any>("42, 24"))[1], test1[1]);

  vector<string> test2;
  test2.push_back("aaa");
  test2.push_back("bbb");
  EXPECT_EQ(any_cast<vector<string> >(lexical_cast<Any>("  aaa,bbb "))[0],
            test2[0]);
  EXPECT_EQ(any_cast<vector<string> >(lexical_cast<Any>("  aaa,bbb "))[1],
            test2[1]);

  EXPECT_ANY_THROW(lexical_cast<Any>("aba caba"));
}
