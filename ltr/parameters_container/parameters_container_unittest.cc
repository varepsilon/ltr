// Copyright 2011 Yandex

#include <gtest/gtest.h>
#include <string>

#include "ltr/parameters_container/parameters_container.h"

using ltr::ParametersContainer;
using std::string;


TEST(ParametersContainerTest, AllInParametersContainerTest) {
  ParametersContainer pc1;
  ParametersContainer pc2;

  pc1.setDouble("double1",10.3);
  pc1.setDouble("double2", 1);
  pc1.setInt("int1",1);
  pc1.setBool("bool1",true);

  EXPECT_EQ(10.3, pc1.getDouble("double1"));
  EXPECT_EQ(1, pc1.getDouble("double2"));
  EXPECT_EQ(1, pc1.getInt("int1"));
  EXPECT_EQ(true, pc1.getBool("bool1"));

  pc2.setInt("int2",2);
  pc2.setDouble("double2",23);

  pc2.copyParameters(pc1);

  EXPECT_EQ(10.3, pc2.getDouble("double1"));
  EXPECT_EQ(23, pc2.getDouble("double2"));
  EXPECT_EQ(1, pc2.getInt("int1"));
  EXPECT_EQ(2, pc2.getInt("int2"));
  EXPECT_EQ(true, pc2.getBool("bool1"));

  pc2.getString();
  pc1.clear();
};
