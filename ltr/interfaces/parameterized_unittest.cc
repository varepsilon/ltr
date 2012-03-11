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
  p.setBool("bool1", false);
  p.setDouble("double1", 3.24);

  EXPECT_EQ(false, p.getBool("bool1"));
  EXPECT_EQ(3.24, p.getDouble("double1"));

  EXPECT_ANY_THROW(p.getDouble("none"));
};

class TestParameterized : public Parameterized {
  public:
    mutable int update_n;
    mutable int check_n;

    TestParameterized() {
      update_n = 0;
      check_n = 0;
    }
    void checkParameters() const {
      check_n++;
      CHECK_INT_PARAMETER("INT1", X > 0);
    }
    void setDefaultParameters() {
      this->addIntParameter("INT1", 10);
      this->addIntParameter("INT2", 5, "GROUP1");
    }
    void updateParameters() {
      update_n++;
    }

  private:
};

TEST(InterfacesTest, ParameterizedGroupsTest) {
  TestParameterized p;
  p.setDefaultParameters();

  EXPECT_EQ(p.getIntParameter("INT1"), 10);
  EXPECT_EQ(p.getIntParameter("INT2", "GROUP1"), 5);

  EXPECT_THROW(p.getIntParameter("INT2"), std::logic_error);
  EXPECT_THROW(p.setIntParameter("INT2", 5), std::logic_error);
  EXPECT_THROW(p.setIntParameter("INT1", 5, "GROUP1"), std::logic_error);
  EXPECT_THROW(p.addIntParameter("INT1", 5), std::logic_error);

  EXPECT_EQ(p.update_n, 0);

  EXPECT_NO_THROW(p.checkParameters());
  EXPECT_EQ(p.check_n, 1);

  EXPECT_THROW(p.setIntParameter("INT1", -1), std::logic_error);
  EXPECT_EQ(p.update_n, 0);
  EXPECT_EQ(p.check_n, 2);

  EXPECT_NO_THROW(p.setIntParameter("INT1", 3));
  EXPECT_EQ(p.update_n, 1);
  EXPECT_EQ(p.check_n, 3);

  ParametersContainer p1 = p.parameters().getGroup("");
  ParametersContainer p2 = p.parameters().getGroup("GROUP1");

  EXPECT_EQ(p1.getInt("INT1"), 3);
  EXPECT_THROW(p1.getInt("INT2"), std::logic_error);

  EXPECT_EQ(p2.getInt("INT2"), 5);
  EXPECT_THROW(p2.getInt("INT1"), std::logic_error);
}
