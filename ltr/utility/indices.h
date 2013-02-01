// Copyright 2011 Yandex

#ifndef LTR_UTILITY_INDICES_H_
#define LTR_UTILITY_INDICES_H_

#include <string>
#include <vector>

#include "ltr/utility/boost/shared_ptr.h"
#include "ltr/utility/boost/lexical_cast.h"

using std::vector;
using std::string;

namespace ltr {
namespace utility {

typedef vector<int> Indices;
typedef ltr::utility::shared_ptr<Indices> IndicesPtr;

typedef vector<int> Permutation;

void getIdPermutation(Permutation *permutation, int size);
void getRandomPermutation(Permutation *permutation, int size);
void getRandomIndices(Indices *indices, int max_index, int count);

Indices getIndicesFromString(const string& string_to_parse);
};
};

#endif  // LTR_UTILITY_INDICES_H_
