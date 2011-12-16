// Copyright 2011 Yandex

#include <gtest/gtest.h>
#include <string>

#include "ltr/interfaces/aliaser.h"

using ltr::Aliaser;
using std::string;


TEST(AliaserTest, AllInAliaserTest) {
  Aliaser a1;
  Aliaser a2("Some alias");
  EXPECT_EQ("Alias", a1.alias());
  EXPECT_EQ("Some alias", a2.alias());
};
