// Copyright 2012 Yandex

#include "gtest/gtest.h"
#include <fstream>

#include "ltr/utility/boost/path.h"

using ltr::utility::FixPathSeparators;
using ltr::utility::AppendTrailingPathSeparator;
using ltr::utility::GetFileName;
using std::ifstream;

TEST(PathTest, TestingSeparatorsFixing) {
  ifstream in;

  in.open(FixPathSeparators("data/tests/test_file.txt").c_str());
  EXPECT_TRUE(in);
  in.close();

  in.open(FixPathSeparators("data\\tests\\test_file.txt").c_str());
  EXPECT_TRUE(in);
  in.close();
}

TEST(PathTest, TestingSeparatorsAppending) {
  ifstream in;

  in.open((AppendTrailingPathSeparator(FixPathSeparators("data/tests")) +
          "test_file.txt").c_str());
  EXPECT_TRUE(in);
  in.close();

  in.open((AppendTrailingPathSeparator(FixPathSeparators("data/tests/")) +
          "test_file.txt").c_str());
  EXPECT_TRUE(in);
  in.close();
}

TEST(PathTest, TestingGettingFilename) {
  string path = AppendTrailingPathSeparator(FixPathSeparators("data/tests")) +
          "test_file.txt";
  EXPECT_STREQ(GetFileName(path).c_str(), "test_file.txt");
}
