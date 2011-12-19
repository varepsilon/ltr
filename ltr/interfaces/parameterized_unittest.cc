// Copyright 2011 Yandex

#include <gtest/gtest.h>
#include <string>

#include "ltr/interfaces/parameterized.h"

using ltr::Parameterized;
using std::string;


TEST(ParameterizedTest, AllInParameterizedTest) {
  Parameterized p;
  p.parameters().setBool("bool1",false);
  p.parameters().setDouble("double1",3.24);
  
  EXPECT_EQ(false, p.parameters().getBool("bool1"));
  EXPECT_EQ(3.24, p.parameters().getDouble("double1"));

  EXPECT_EQ("",p.checkParameters());
  p.setDefaultParameters();
};
