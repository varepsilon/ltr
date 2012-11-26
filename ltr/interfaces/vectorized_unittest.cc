// Copyright 2012 Yandex

#include <gtest/gtest.h>

#include <vector>

#include "ltr/interfaces/vectorized.h"

using ltr::Vectorized;
using std::vector;

class VectorInheritanceTestClass: public Vectorized<char> {
  private:
    vector<char> features_;
  public:
    VectorInheritanceTestClass(): Vectorized<char>(features_) {}
};

TEST(InterfacesTest, VectorizedTest) {
  VectorInheritanceTestClass test_object;
  // test_object: []
  EXPECT_EQ(test_object.count(), 0);

  EXPECT_NO_THROW(test_object.add('a'));
  EXPECT_NO_THROW(test_object.add('a'));
  EXPECT_NO_THROW(test_object.add('a'));
  // test_object: ['a', 'a', 'a']
  EXPECT_EQ(test_object.count(), 3);
  EXPECT_EQ(test_object[2], 'a');
  EXPECT_THROW(test_object.at(3), std::out_of_range);

  EXPECT_NO_THROW(test_object.add('b'));
  // test_object: ['a', 'a', 'a', 'b']
  EXPECT_EQ(test_object.count(), 4);
  EXPECT_EQ(test_object[0], 'a');
  EXPECT_EQ(test_object.at(3), 'b');
  EXPECT_THROW(test_object.at(4), std::out_of_range);

  EXPECT_NO_THROW(test_object[1] = 'c');
  // test_object: ['a', 'c', 'a', 'b']
  EXPECT_EQ(test_object.count(), 4);
  EXPECT_EQ(test_object[1], 'c');
  EXPECT_EQ(test_object[0], 'a');

  EXPECT_NO_THROW(test_object.erase(1));
  // test_object: ['a', 'a', 'b']
  EXPECT_EQ(test_object.count(), 3);
  EXPECT_EQ(test_object[1], 'a');
  EXPECT_THROW(test_object.erase(4), std::out_of_range);

  for (VectorInheritanceTestClass::iterator i = test_object.begin();
      i != test_object.end();
      ++i) {
    if (*i == 'a')
      *i = 'b';
    else if (*i == 'b')
      *i = 'a';
    else
      ADD_FAILURE();
  }
  // test_object: ['b', 'b', 'a']
  EXPECT_EQ(test_object.count(), 3);
  EXPECT_EQ(test_object[0], 'b');
  EXPECT_EQ(test_object[1], 'b');
  EXPECT_EQ(test_object[2], 'a');

  EXPECT_NO_THROW(test_object.clear());
  // test_object: []
  EXPECT_EQ(test_object.count(), 0);
}
