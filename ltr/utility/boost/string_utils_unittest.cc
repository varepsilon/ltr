// Copyright 2012 Yandex

#include "gtest/gtest.h"
#include <string>
#include <vector>

#include "ltr/utility/boost/string_utils.h"

using ltr::utility::trim_copy;
using ltr::utility::trim;
using ltr::utility::split;
using ltr::utility::to_upper;
using ltr::utility::to_lower;
using ltr::utility::erase_all;
using std::string;
using std::vector;

TEST(StringUtilsTest, TestingTrim) {
  EXPECT_EQ(trim_copy(""), string(""));
  EXPECT_EQ(trim_copy("        "), string(""));
  EXPECT_EQ(trim_copy("\t"), string(""));
  EXPECT_EQ(trim_copy(" aba c aba\n "), string("aba c aba"));
  EXPECT_EQ(trim_copy("1"), string("1"));
  EXPECT_EQ(trim_copy("\taba\t"), string("aba"));
  EXPECT_EQ(trim_copy("yabracadabra", "ay"), string("bracadabr"));
  EXPECT_EQ(trim_copy("aaaaaaaaaaaaa", "a"), string(""));

  string test = "";
  trim(&test);
  EXPECT_EQ(test, string(""));
  test = "        ";
  trim(&test);
  EXPECT_EQ(test, string(""));
  test = "\t";
  trim(&test);
  EXPECT_EQ(test, string(""));
  test = " aba c aba\n ";
  trim(&test);
  EXPECT_EQ(test, string("aba c aba"));
  test = "1";
  trim(&test);
  EXPECT_EQ(test, string("1"));
  test = "yabracadabra";
  trim(&test, "ay");
  EXPECT_EQ(test, string("bracadabr"));
  test = "aaaaaaaaaaaaa";
  trim(&test, "a");
  EXPECT_EQ(test, string(""));
  
  vector<string> expected_result;
  vector<string> input;
  vector<string> result;
  EXPECT_EQ(trim_copy(input), expected_result);
  trim_copy(input, &result);
  EXPECT_EQ(result, expected_result);
  trim(&input);
  EXPECT_EQ(input, expected_result);
  input.push_back(" aaa  ");
  input.push_back("   ");
  input.push_back("aaa  aaa");
  expected_result.push_back("aaa");
  expected_result.push_back("");
  expected_result.push_back("aaa  aaa");
  EXPECT_EQ(trim_copy(input), expected_result);
  trim_copy(input, &result);
  EXPECT_EQ(result, expected_result);
  trim(&input);
  EXPECT_EQ(input, expected_result);
}

TEST(StringUtilsTest, TestingSplit) {
  vector<string> expected_result;
  vector<string> result;
  vector<string> input;

  expected_result.clear();
  expected_result.push_back("");
  split("", "<-", &result);
  EXPECT_EQ(result, expected_result);
  expected_result.push_back("");
  split("<-", "<-", &result);
  EXPECT_EQ(result, expected_result);
  expected_result.push_back("a");
  expected_result.push_back("cab");
  split("::::a::cab", "::", &result);
  EXPECT_EQ(result, expected_result);
  expected_result.push_back("ab::ba");
  split("::::a::cab::ab::ba", "::", &result, 4);
  EXPECT_EQ(result, expected_result);
  EXPECT_EQ(split("::::a::cab::ab::ba", "::", 4), expected_result);

  expected_result.clear();
  split("", &result);
  EXPECT_EQ(result, expected_result);
  split(" \t\n   \r\n    \t", &result);
  EXPECT_EQ(result, expected_result);
  expected_result.push_back("a");
  expected_result.push_back("b");
  split("  a\r\t\nb   ", &result);
  EXPECT_EQ(result, expected_result);
  EXPECT_EQ(split("  a\r\t\nb   "), expected_result);

  input.clear();
  input.push_back("a->b->");
  input.push_back("->a->b");
  expected_result.clear();
  expected_result.push_back("a");
  expected_result.push_back("b");
  expected_result.push_back("");
  expected_result.push_back("");
  expected_result.push_back("a");
  expected_result.push_back("b");
  split(input, "->", &result);
  EXPECT_EQ(result, expected_result);
  input.push_back("a->b->c");
  expected_result.push_back("a");
  expected_result.push_back("b->c");
  split(input, "->", &result, 5);
  EXPECT_EQ(split(input, "->", 5), expected_result);

  input.clear();
  input.push_back("a  b  ");
  input.push_back("  b a");
  input.push_back("a b c");
  expected_result.clear();
  expected_result.push_back("a");
  expected_result.push_back("b");
  expected_result.push_back("b");
  expected_result.push_back("a");
  expected_result.push_back("a");
  expected_result.push_back("b c");
  split(input, &result, 3);
  EXPECT_EQ(split(input, 3), expected_result);

  input.clear();
  input.push_back("a->b");
  input.push_back("b->a");
  expected_result.clear();
  expected_result.push_back("a");
  expected_result.push_back("b");
  expected_result.push_back("b");
  expected_result.push_back("a");
  split(&input, "->");
  EXPECT_EQ(input, expected_result);

  input.clear();
  input.push_back("a b");
  input.push_back("b a");
  expected_result.clear();
  expected_result.push_back("a");
  expected_result.push_back("b");
  expected_result.push_back("b");
  expected_result.push_back("a");
  split(&input);
  EXPECT_EQ(input, expected_result);
}

TEST(StringUtilsTest, TestingToUpper) {
  string str = "";
  to_upper(&str);
  EXPECT_EQ(str, "");
  str = "aba caba";
  to_upper(&str);
  EXPECT_EQ(str, "ABA CABA");
}

TEST(StringUtilsTest, TestingToLower) {
  string str = "";
  to_lower(&str);
  EXPECT_EQ(str, "");
  str = "aBA CAba";
  to_lower(&str);
  EXPECT_EQ(str, "aba caba");
}

TEST(StringUtilsTest, TestingEraseAll) {
  string str = "";
  erase_all(&str, "");
  EXPECT_EQ(str, "");
  erase_all(&str, "aba");
  EXPECT_EQ(str, "");
  str = "aba caba";
  erase_all(&str, "a");
  EXPECT_EQ(str, "b cb");
  str = "aba caba";
  erase_all(&str, "ab");
  EXPECT_EQ(str, "a ca");
  str = "aba caba";
  erase_all(&str, "aba caba");
  EXPECT_EQ(str, "");
}
