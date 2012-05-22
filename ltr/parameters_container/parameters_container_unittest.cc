// Copyright 2011 Yandex

#include <gtest/gtest.h>
#include <string>

#include "ltr/parameters_container/parameters_container.h"

using ltr::ParametersContainer;
using std::string;


TEST(ParametersContainerTest, AllInParametersContainerTest) {
  ParametersContainer pc1;
  ParametersContainer pc2;

  pc1.Set("double1", 10.3);
  pc1.Set<double>("double2", 1);
  pc1.Set("int1", 1);
  pc1.Set("bool1", true);

  EXPECT_EQ(10.3, pc1.Get<double>("double1"));
  EXPECT_EQ(1, pc1.Get<double>("double2"));
  EXPECT_EQ(1, pc1.Get<int>("int1"));
  EXPECT_EQ(true, pc1.Get<bool>("bool1"));

  EXPECT_ANY_THROW(pc1.Get<bool>("none"));

  pc2.Set("int2", 2);
  pc2.Set<double>("double2", 23);

  EXPECT_ANY_THROW(pc2.Copy(pc1));

  pc2.Set<double>("double1", 0);
  pc2.Set("int1", 0);
  pc2.Set("bool1", false);
  pc2.Copy(pc1);

  EXPECT_EQ(10.3, pc2.Get<double>("double1"));
  EXPECT_EQ(1, pc2.Get<double>("double2"));
  EXPECT_EQ(1, pc2.Get<int>("int1"));
  EXPECT_EQ(2, pc2.Get<int>("int2"));
  EXPECT_EQ(true, pc2.Get<bool>("bool1"));

  pc2.ToString();
  pc1.Clear();
};
