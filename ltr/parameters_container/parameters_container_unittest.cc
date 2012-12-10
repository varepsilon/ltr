// Copyright 2011 Yandex

#include <gtest/gtest.h>
#include <string>
#include <vector>

#include "ltr/parameters_container/parameters_container.h"

using std::string;
using std::vector;

using ltr::ParametersContainer;

TEST(ParametersContainerTest, AllInParametersContainerTest) {
  ParametersContainer parameters_container1;
  ParametersContainer parameters_container2;

  parameters_container1.Set("double1", 10.3);
  parameters_container1.Set<double>("double2", 1);
  parameters_container1.Set("int1", 1);
  parameters_container1.Set("bool1", true);

  EXPECT_EQ(10.3, parameters_container1.Get<double>("double1"));
  EXPECT_EQ(1, parameters_container1.Get<double>("double2"));
  EXPECT_EQ(1, parameters_container1.Get<int>("int1"));
  EXPECT_EQ(true, parameters_container1.Get<bool>("bool1"));

  EXPECT_ANY_THROW(parameters_container1.Get<bool>("none"));

  parameters_container2.Set("int2", 2);
  parameters_container2.Set<double>("double2", 23);

  EXPECT_ANY_THROW(parameters_container2.Copy(parameters_container1));

  parameters_container2.Set<double>("double1", 0);
  parameters_container2.Set("int1", 0);
  parameters_container2.Set("bool1", false);
  parameters_container2.Copy(parameters_container1);

  EXPECT_EQ(10.3, parameters_container2.Get<double>("double1"));
  EXPECT_EQ(1, parameters_container2.Get<double>("double2"));
  EXPECT_EQ(1, parameters_container2.Get<int>("int1"));
  EXPECT_EQ(2, parameters_container2.Get<int>("int2"));
  EXPECT_EQ(true, parameters_container2.Get<bool>("bool1"));

  parameters_container2.toString();
  parameters_container1.Clear();
};

TEST(ParametersContainerTest, NonScalarParametersTest) {
  ParametersContainer parameters_container1;
  ParametersContainer parameters_container2;

  parameters_container1.Set<vector<int> >("vector1", vector<int>(3, 3));
  parameters_container1.Set<vector<double> >("vector2", vector<double>(4, 5));

  EXPECT_EQ(vector<int>(3, 3),
            parameters_container1.Get<vector<int> >("vector1"));
  EXPECT_EQ(vector<double>(4, 5),
            parameters_container1.Get<vector<double> >("vector2"));

  EXPECT_ANY_THROW(parameters_container1.Get<vector<bool> >("none"));

  parameters_container2.Set<vector<string> >("vector4",
                                             vector<string>(10, "iddqd"));
  parameters_container2.Set<vector<bool> >("vector3",
                                           vector<bool>(5, true));
  EXPECT_ANY_THROW(parameters_container2.Copy(parameters_container1));

  EXPECT_EQ(vector<string>(10, "iddqd"),
            parameters_container2.Get<vector<string> >("vector4"));
  EXPECT_EQ(vector<bool>(5, true),
            parameters_container2.Get<vector<bool> >("vector3"));

  EXPECT_NO_THROW(parameters_container1.toString());
  EXPECT_NO_THROW(parameters_container2.toString());
  EXPECT_NO_THROW(parameters_container1.Clear());
  EXPECT_NO_THROW(parameters_container2.Clear());
}

TEST(ParametersContainerTest, DefaultParameterValuesTest) {
  ParametersContainer parameters_container;

  EXPECT_EQ(10.3, parameters_container.Get<double>("double1", 10.3));
  EXPECT_EQ(vector<int>(3, 3),
            parameters_container.Get<vector<int> >("vector1", vector<int>(3, 3)));
}
