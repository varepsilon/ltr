// Copyright 2011 Yandex

#include <gtest/gtest.h>
#include <string>

#include "ltr/parameters_container/parameters_container.h"

using ltr::ParametersContainer;
using std::string;


TEST(InterfacesTest, ParameterizedTest) {
  ParametersContainer p;
  p.setBool("bool1", false);
  p.setDouble("double1", 3.24);

  EXPECT_EQ(false, p.getBool("bool1"));
  EXPECT_EQ(3.24, p.getDouble("double1"));

  EXPECT_ANY_THROW(p.getDouble("none"));
};
