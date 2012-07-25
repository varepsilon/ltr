// Copyright 2012 Yandex School Practice

#include <map>

#include "gtest/gtest.h"

#include "ltr/utility/indices.h"

using std::map;
using std::sort;

using ltr::utility::Indices;
using ltr::utility::getIdPermutation;
using ltr::utility::getIndicesFromString;
using ltr::utility::getRandomPermutation;
using ltr::utility::getRandomIndices;
using ltr::utility::Permutation;

TEST(IndicesTest, GetIdPermutationTest) {
  int N = 100;
  Permutation permutation;
  getIdPermutation(&permutation, N);

  for (int index = 0; index < N; ++index) {
    EXPECT_EQ(index, permutation[index]);
  }
}

int Factorial(int number) {
  int result = 1;
  for (int index = 1; index <= number; ++index) {
    result *= index;
  }
  return result;
}

TEST(IndicesTest, GetRandomPermutationTest) {
  const int N = 5;
  const int number_of_permutations = Factorial(N);

  map<Permutation, int> permutations_counter;
  for (int permutation_index = 0;
       permutation_index < 1000 * number_of_permutations;
       ++permutation_index) {
    Permutation permutation;
    getRandomPermutation(&permutation, N);
    permutations_counter[permutation]++;
  }

  EXPECT_EQ(number_of_permutations, permutations_counter.size());

  for (map<Permutation, int>::iterator map_iterator =
         permutations_counter.begin();
       map_iterator != permutations_counter.end();
       ++map_iterator) {
    EXPECT_TRUE(800 <= map_iterator->second && map_iterator->second <= 1200);
  }
}

TEST(IndicesTest, GetRandomIndicesTest) {
  Indices indices;
  int max_index = 1000;
  int size = 100;
  getRandomIndices(&indices, max_index, size);
  EXPECT_EQ(indices.size(), size);

  for (int index = 0; index < size; ++index) {
    EXPECT_TRUE(indices[index] < max_index);
  }
  sort(indices.begin(), indices.end());
  indices.resize(std::unique(indices.begin(), indices.end()) -
                 indices.begin());
  EXPECT_EQ(size, indices.size());
}

TEST(IndicesTest, GetIndicesFromStringTest) {
  string given_string;
  EXPECT_ANY_THROW(getIndicesFromString(given_string));

  given_string = "12-11";
  EXPECT_ANY_THROW(getIndicesFromString(given_string));

  given_string = "11-12-13";
  EXPECT_ANY_THROW(getIndicesFromString(given_string));

  given_string = "-12-11";
  EXPECT_ANY_THROW(getIndicesFromString(given_string));

  given_string = "1-11,";
  EXPECT_ANY_THROW(getIndicesFromString(given_string));

  given_string = "12, 11, , 14";
  EXPECT_ANY_THROW(getIndicesFromString(given_string));

  given_string = "103";
  EXPECT_EQ(Indices(1, 103), getIndicesFromString(given_string));

  given_string = "103, 103, 103";
  EXPECT_EQ(Indices(1, 103), getIndicesFromString(given_string));

  given_string = "103, 103 - 103, 103,103-103";
  EXPECT_EQ(Indices(1, 103), getIndicesFromString(given_string));

  given_string = "7-9,6-8,4,1624-1630,1631,22,27-27";
  Indices indices;
  indices.push_back(4);
  for (int index = 6; index <= 9; ++index)
    indices.push_back(index);
  indices.push_back(22);
  indices.push_back(27);
  for (int index = 1624; index <= 1631; ++index)
    indices.push_back(index);
  EXPECT_EQ(indices, getIndicesFromString(given_string));
}
