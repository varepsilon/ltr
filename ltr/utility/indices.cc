// Copyright 2011 Yandex

#include <algorithm>

#include "ltr/utility/indices.h"
#include "ltr/utility/random_number_generator.h"

using std::random_shuffle;

using ltr::utility::randomizer;

namespace ltr {
namespace utility {

void getIdPermutation(Permutation *permution, int size) {
  permution->resize(size);
  for (int i = 0; i <size; ++i) {
    permution->at(i) = i;
  }
}

void getRandomPermutation(Permutation *permution, int size) {
  getIdPermutation(permution, size);
  random_shuffle(permution->begin(), permution->end(), randomizer);
}

void getRandomIndices(Indices *indices, int max_index, int count) {
  indices->resize(count);
  Permutation permutation;
  getRandomPermutation(&permutation, max_index);
  copy(permutation.begin(), permutation.begin() + count, indices->begin());
}
};
};