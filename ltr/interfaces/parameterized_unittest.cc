// Copyright 2012 Yandex

#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "ltr/parameters_container/parameters_container.h"

#include "ltr/interfaces/parameterized.h"

using ltr::ParametersContainer;
using ltr::Parameterized;
using std::string;

TEST(InterfacesTest, ParameterizedTest) {
  ParametersContainer parameters_container;
  parameters_container.Set("bool1", false);
  parameters_container.Set("double1", 3.24);

  EXPECT_EQ(false, parameters_container.Get<bool>("bool1"));
  EXPECT_EQ(3.24, parameters_container.Get<double>("double1"));

  EXPECT_ANY_THROW(parameters_container.Get<double>("none"));
};

class GetSetTestClass {
  private:
    std::vector<char> test_vector_;
  public:
    GET_SET_VECTOR(char, test_vector)
};

TEST(InterfacesTest, GetSetVectorMacroTest) {
  GetSetTestClass test_object;
  EXPECT_EQ(test_object.test_vector_size(), 0);

  EXPECT_NO_THROW(test_object.set_test_vector(std::vector<char>(3, 'a')));
  EXPECT_EQ(test_object.test_vector_size(), 3);
  EXPECT_EQ(test_object.test_vector(2), 'a');
  EXPECT_THROW(test_object.test_vector(3), std::out_of_range);

  EXPECT_NO_THROW(test_object.add_test_vector('b'));
  EXPECT_EQ(test_object.test_vector_size(), 4);
  EXPECT_EQ(test_object.test_vector(0), 'a');
  EXPECT_EQ(test_object.test_vector(3), 'b');
  EXPECT_THROW(test_object.test_vector(4), std::out_of_range);

  EXPECT_NO_THROW(test_object.test_vector(1) = 'c');
  EXPECT_EQ(test_object.test_vector(1), 'c');
  EXPECT_EQ(test_object.test_vector(0), 'a');
  std::vector<char> must_equal(4, 'a');
  must_equal[1] = 'c';
  must_equal[3] = 'b';
  EXPECT_EQ(test_object.test_vector(), must_equal);
}
