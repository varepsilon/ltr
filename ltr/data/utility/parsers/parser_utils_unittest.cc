// Copyright 2013 Yandex

#include <vector>
#include <string>

#include "gtest/gtest.h"

#include "ltr/data/utility/parsers/parser_utils.h"

using std::vector;
using std::string;

using ltr::io_utility::findFirstUnscreened;
using ltr::io_utility::screenSpecialChars;
using ltr::io_utility::unscreenSpecialChars;
using ltr::io_utility::applyQuoting;
using ltr::io_utility::escapeSplit;

TEST(ParserUtilsTest, TestingFindFirstUnscreened) {
  EXPECT_EQ(findFirstUnscreened("find \\: \\: : \\:", ':'), 11);
  EXPECT_EQ(findFirstUnscreened("find \\: \\\\: :", ':'), 10);
  EXPECT_EQ(findFirstUnscreened("find", ':'), -1);
  EXPECT_EQ(findFirstUnscreened(": \\: : :", ':', 1), 5);
}

TEST(ParserUtilsTest, TestingScreenSpecialChars) {
  string result;

  screenSpecialChars("find \"' \\\\", &result);
  EXPECT_EQ(result, string("find \\\"\\' \\\\\\\\"));

  screenSpecialChars("find t \\l :m", &result, " \\:l", ':');
  EXPECT_EQ(result, string("find: t: :\\:l: ::m"));
}

TEST(ParserUtilsTest, TestingUnscreenSpecialChars) {
  string result;

  unscreenSpecialChars("find \\t \\  \\\\l \\\\\\m", &result);
  EXPECT_EQ(result, string("find t   \\l \\m"));

  unscreenSpecialChars("find :t :  ::l :::m :\\", &result, ':');
  EXPECT_EQ(result, string("find t   :l :m \\"));
}

TEST(ParserUtilsTest, TestingApplyQuoting) {
  string result;

  applyQuoting("test", &result);
  EXPECT_EQ(result, string("test"));

  applyQuoting("test test", &result);
  EXPECT_EQ(result, string("\"test test\""));

  applyQuoting("\"test\"", &result, '[', ']');
  EXPECT_EQ(result, string("[\\\"test\\\"]"));
}

TEST(ParserUtilsTest, TestingEscapeSplit) {
  string str = "test1, test2, \"test 3\", \"test \\\\4\"";
  vector<string> expected, parsed;
  expected.push_back("test1");
  expected.push_back("test2");
  expected.push_back("test 3");
  expected.push_back("test \\4");
  escapeSplit(str, &parsed);
  EXPECT_EQ(expected, parsed);
}
