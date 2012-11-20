// Copyright 2012 Yandex

#include "ltr/utility/indices.h"

#include <boost/algorithm/string.hpp>
#include "ltr/utility/boost/lexical_cast.h"

#include <algorithm>

#include "ltr/utility/random_number_generator.h"

using std::logic_error;
using std::random_shuffle;

using boost::erase_all;

using ltr::utility::randomizer;

namespace ltr {
namespace utility {

void getIdPermutation(Permutation *permutation, int size) {
  permutation->resize(size);
  for (int i = 0; i <size; ++i) {
    permutation->at(i) = i;
  }
}

void getRandomPermutation(Permutation *permutation, int size) {
  getIdPermutation(permutation, size);
  random_shuffle(permutation->begin(), permutation->end(), randomizer);
}

void getRandomIndices(Indices *indices, int max_index, int count) {
  indices->resize(count);
  Permutation permutation;
  getRandomPermutation(&permutation, max_index);
  copy(permutation.begin(), permutation.begin() + count, indices->begin());
}

Indices getIndicesFromString(const string& string_to_parse) {
  string given_string = string_to_parse;
  erase_all(given_string, " ");

  if (given_string.empty()) {
    throw logic_error("String is empty");
  }

  vector<string> lexemes;
  boost::split(lexemes, given_string, is_any_of(","));
  vector<int> result;
  for (int lexeme_index = 0;
       lexeme_index < (int)lexemes.size();
       ++lexeme_index) {
    string lexeme = lexemes[lexeme_index];
    if (lexeme.empty()) {
      throw logic_error("Invalid string format");
    }

    if (!isdigit(lexeme[0]) || !isdigit(lexeme[lexeme.size() - 1])) {
      throw logic_error("Invalid string format");
    }

    vector<string> numbers;
    boost::split(numbers, lexeme, is_any_of("-"));

    if (numbers.size() == 0 || numbers.size() > 2) {
      throw logic_error("Invalid string format");
    }
    if (numbers.size() == 1) {
      result.push_back(ltr::utility::lexical_cast<int>(numbers[0]));
    } else {
      int left_border = ltr::utility::lexical_cast<int>(numbers[0]);
      int right_border = ltr::utility::lexical_cast<int>(numbers[1]);
      if (left_border > right_border) {
        throw logic_error("Invalid string format");
      }
      for (int number = left_border; number <= right_border; ++number) {
        result.push_back(number);
      }
    }
  }

  std::sort(result.begin(), result.end());
  result.resize(std::unique(result.begin(), result.end()) - result.begin());
  return result;
}
};
};
