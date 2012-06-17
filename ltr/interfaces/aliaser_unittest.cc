// Copyright 2011 Yandex

#include <gtest/gtest.h>
#include <string>

#include "ltr/interfaces/aliaser.h"

using ltr::Aliaser;
using std::string;

class AliaserDerived : public Aliaser {
 private:
  virtual string getDefaultAlias() const {return "AliaserDerived";}
};


TEST(InterfacesTest, AliaserTest) {
  AliaserDerived a;
  EXPECT_EQ("AliaserDerived", a.alias());
  a.set_alias("Some alias");
  EXPECT_EQ("Some alias", a.alias());
};
