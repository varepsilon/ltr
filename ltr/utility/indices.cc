// Copyright 2011 Yandex

#include "ltr/utility/indices.h"

#include <algorithm>

using std::random_shuffle;

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
  random_shuffle(permution->begin(), permution->end());
}

void getRandomIndices(Indices *indices, int max_index, int count) {
  indices->resize(count);
  Permutation permutation;
  getRandomPermutation(&permutation, max_index);
  copy(permutation.begin(), permutation.begin() + count, indices->begin());
}
};
};
