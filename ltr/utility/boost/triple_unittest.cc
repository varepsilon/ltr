// Copyright 2012 Yandex

#include "gtest/gtest.h"

#include <string>

#include "ltr/utility/boost/triple.h"

using std::string;
using ltr::utility::triple;
using ltr::utility::make_triple;

class T{
 public:
  T(int v){}
 private:
  T() {}
};

TEST(TripleTest, TestingTriple) {
  triple<double, int, char> t0(1.0, 2, 'a');
  triple<double, int, char> t = t0;
  EXPECT_EQ(t.get_0(), 1.0);
  EXPECT_EQ(t.get_1(), 2);
  EXPECT_EQ(t.get_2(), 'a');
  EXPECT_EQ(make_triple(1.0, 0, "ololo").get_0(), 1.0);
  EXPECT_EQ(make_triple(1.0, 0, "ololo").get_1(), 0);
  EXPECT_EQ(make_triple(1.0, 0, string("ololo")).get_2(), "ololo");

  EXPECT_EQ(make_triple(1.0, 0, string("ololo")) <
    make_triple(2.0, 0, string("ololo")), true);
  EXPECT_EQ(make_triple(1.0, 0, string("ololo")) <
    make_triple(1.0, 10, string("ololo")), true);
  EXPECT_EQ(make_triple(1.0, 0, string("ololo")) <
    make_triple(1.0, 0, string("ololp")), true);
  EXPECT_EQ(make_triple(1.0, 0, string("ololo")) >
    make_triple(-2.0, 0, string("ololo")), true);
  EXPECT_EQ(make_triple(1.0, 0, string("ololo")) >
    make_triple(1.0, -1, string("ololo")), true);
  EXPECT_EQ(make_triple(1.0, 0, string("ololo")) >
    make_triple(1.0, 0, string("ololn")), true);

  EXPECT_EQ(make_triple(1.0, 0, string("ololo")) >
    make_triple(2.0, 0, string("ololo")), false);
  EXPECT_EQ(make_triple(1.0, 0, string("ololo")) >
    make_triple(1.0, 10, string("ololo")), false);
  EXPECT_EQ(make_triple(1.0, 0, string("ololo")) >
    make_triple(1.0, 0, string("ololp")), false);
  EXPECT_EQ(make_triple(1.0, 0, string("ololo")) <
    make_triple(-2.0, 0, string("ololo")), false);
  EXPECT_EQ(make_triple(1.0, 0, string("ololo")) <
    make_triple(1.0, -1, string("ololo")), false);
  EXPECT_EQ(make_triple(1.0, 0, string("ololo")) <
    make_triple(1.0, 0, string("ololn")), false);

  EXPECT_EQ(make_triple(1.0, 0, string("ololo")) ==
    make_triple(2.0, 0, string("ololo")), false);
  EXPECT_EQ(make_triple(1.0, 0, string("ololo")) ==
    make_triple(1.0, 10, string("ololo")), false);
  EXPECT_EQ(make_triple(1.0, 0, string("ololo")) ==
    make_triple(1.0, 0, string("ololp")), false);
  EXPECT_EQ(make_triple(1.0, 0, string("ololo")) ==
    make_triple(1.0, 0, string("ololo")), true);
}
