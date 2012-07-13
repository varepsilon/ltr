// Copyright 2011 Yandex

#include <gtest/gtest.h>
#include <string>

#include "ltr/parameters_container/parameters_container.h"
#include "ltr/interfaces/parameterized.h"

using ltr::ParametersContainer;
using ltr::Parameterized;
using std::string;


TEST(InterfacesTest, ParameterizedTest) {
  ParametersContainer p;
  p.Set("bool1", false);
  p.Set("double1", 3.24);

  EXPECT_EQ(false, p.Get<bool>("bool1"));
  EXPECT_EQ(3.24, p.Get<double>("double1"));

  EXPECT_ANY_THROW(p.Get<double>("none"));
};