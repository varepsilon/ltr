// Copyright 2011 Yandex

#include <gtest/gtest.h>
#include <string>

#include "ltr/interfaces/aliaser.h"

using ltr::Aliaser;
using std::string;

class AliaserDerived : public Aliaser {
};


TEST(InterfacesTest, AliaserTest) {
  Aliaser a1;
  Aliaser a2;
  a2.set_alias("Some alias");
  EXPECT_EQ("class ltr::Aliaser", a1.alias());
  EXPECT_EQ("Some alias", a2.alias());
  AliaserDerived a3;
  EXPECT_EQ("class AliaserDerived", a3.alias());
};
