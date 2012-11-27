// Copyright 2012 Yandex

#include <gtest/gtest.h>

#include <vector>
#include <stdexcept>

#include "ltr/utility/macros.h"
#include "ltr/utility/shared_ptr.h"

using std::vector;
using ltr::utility::shared_ptr;

class GetSetVectorTestClass {
  private:
    std::vector<char> test_vector_;
  public:
    GET_SET_VECTOR(char, test_vector)
};

TEST(UtilityTest, GetSetVectorMacrosTest) {
  GetSetVectorTestClass test_object;
  EXPECT_EQ(test_object.test_vector_count(), 0);

  EXPECT_NO_THROW(test_object.set_test_vector(std::vector<char>(3, 'a')));
  EXPECT_EQ(test_object.test_vector_count(), 3);
  EXPECT_EQ(test_object.test_vector(2), 'a');
  EXPECT_THROW(test_object.test_vector(3), std::out_of_range);

  EXPECT_NO_THROW(test_object.add_test_vector('b'));
  EXPECT_EQ(test_object.test_vector_count(), 4);
  EXPECT_EQ(test_object.test_vector(0), 'a');
  EXPECT_EQ(test_object.test_vector(3), 'b');
  EXPECT_THROW(test_object.test_vector(4), std::out_of_range);

  EXPECT_NO_THROW(test_object.test_vector(1) = 'c');
  EXPECT_EQ(test_object.test_vector_count(), 4);
  EXPECT_EQ(test_object.test_vector(1), 'c');
  EXPECT_EQ(test_object.test_vector(0), 'a');
  std::vector<char> must_equal(4, 'a');
  must_equal[1] = 'c';
  must_equal[3] = 'b';
  EXPECT_EQ(test_object.test_vector(), must_equal);
}

class GetSetVectorOfPtrTestClass {
  private:
    std::vector<shared_ptr<char> > test_vector_;
  public:
    GET_SET_VECTOR_OF_PTR(char, test_vector)
};

TEST(UtilityTest, GetSetVectorOfPtrMacrosTest) {
  GetSetVectorOfPtrTestClass test_object;
  EXPECT_EQ(test_object.test_vector_count(), 0);

  EXPECT_NO_THROW(test_object.add_test_vector(new char('a'))); //NOLINT
  EXPECT_NO_THROW(test_object.add_test_vector(new char('a'))); //NOLINT
  EXPECT_NO_THROW(test_object.add_test_vector(new char('a'))); //NOLINT
  EXPECT_EQ(test_object.test_vector_count(), 3);
  EXPECT_EQ(test_object.test_vector(2), 'a');
  EXPECT_THROW(test_object.test_vector(3), std::out_of_range);

  EXPECT_NO_THROW(test_object.add_test_vector(new char('b'))); //NOLINT
  EXPECT_EQ(test_object.test_vector_count(), 4);
  EXPECT_EQ(test_object.test_vector(0), 'a');
  EXPECT_EQ(test_object.test_vector(3), 'b');
  EXPECT_THROW(test_object.test_vector(4), std::out_of_range);

  EXPECT_NO_THROW(test_object.test_vector(1) = 'c');
  EXPECT_EQ(test_object.test_vector_count(), 4);
  EXPECT_EQ(test_object.test_vector(1), 'c');
  EXPECT_EQ(test_object.test_vector(0), 'a');

  EXPECT_NE(test_object.test_vector_ptr(1), shared_ptr<char>());
  EXPECT_NO_THROW(test_object.add_test_vector(test_object.test_vector_ptr(1)));
  EXPECT_EQ(test_object.test_vector_count(), 5);
  EXPECT_EQ(test_object.test_vector(4), 'c');
  EXPECT_NO_THROW(test_object.test_vector(4) = 'd');
  EXPECT_EQ(test_object.test_vector(1), 'd');
}

class AbstractTestClass {
  public:
    virtual int pure_function() = 0;
    virtual ~AbstractTestClass() {}
};

class ConcreteTestClass: public AbstractTestClass {
  public:
    virtual int pure_function() {
      return 15;
    }
};

class GetSetVectorOfAbstractPtrTestClass {
  private:
    vector<shared_ptr<AbstractTestClass> > test_vector_;
  public:
    GET_SET_VECTOR_OF_PTR(AbstractTestClass, test_vector);
};

TEST(UtilityTest, GetSetVectorOfAbstractPtrMacrosTest) {
  GetSetVectorOfAbstractPtrTestClass test_object;
  EXPECT_NO_THROW(test_object.add_test_vector(new ConcreteTestClass));
  EXPECT_EQ(test_object.test_vector(0).pure_function(), 15);
}

TEST(UtilityTest, CheckMacrosTest) {
  EXPECT_NO_THROW(CHECK(2 + 2 == 4));
  EXPECT_THROW(CHECK(2 * 2 == 42), std::logic_error);
}
