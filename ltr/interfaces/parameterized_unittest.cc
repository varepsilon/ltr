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
      this->checkParameter<int>("INT1", std::bind2nd(std::greater<int>(), 0));  // NOLINT
    }
    void setDefaultParameters() {
      addNewParam("INT1", 10);
      // parameters().AddNew("INT2", 5, "GROUP1"); //variant->any_TODO
    }
    void parametersUpdateCallback() {
      update_n++;
    }

  private:
};

TEST(InterfacesTest, ParameterizedGroupsTest) {
  TestParameterized p;
  p.setDefaultParameters();

  EXPECT_EQ(p.parameters().Get<int>("INT1"), 10);
  // EXPECT_EQ(p.getIntParameter("INT2", "GROUP1"), 5); //variant->any_TODO

  EXPECT_THROW(p.parameters().Get<int>("INT2"), std::logic_error);
  // EXPECT_THROW(p.setIntParameter("INT2", 5),
  // std::logic_error); //variant->any_TODO
  // EXPECT_THROW(p.SetExistingParameter("INT1", 5,
  // "GROUP1"), std::logic_error);
  EXPECT_THROW(p.addNewParam("INT1", 5), std::logic_error);

  EXPECT_EQ(p.update_n, 0);

  EXPECT_NO_THROW(p.checkParameters());
  EXPECT_EQ(p.check_n, 1);

  EXPECT_THROW(p.setExistingParameter<int>("INT1", -1), std::logic_error);
  EXPECT_EQ(p.update_n, 0);
  EXPECT_EQ(p.check_n, 2);

  EXPECT_NO_THROW(p.setExistingParameter<int>("INT1", 3));
  EXPECT_EQ(p.update_n, 1);
  EXPECT_EQ(p.check_n, 3);

  // variant->any_TODO
  // ParametersContainer p1 = p.parameters().getGroup("");
  // ParametersContainer p2 = p.parameters().getGroup("GROUP1");

  // EXPECT_EQ(p1.getInt("INT1"), 3);
  // EXPECT_THROW(p1.getInt("INT2"), std::logic_error);

  // EXPECT_EQ(p2.getInt("INT2"), 5);
  // EXPECT_THROW(p2.getInt("INT1"), std::logic_error);
}
