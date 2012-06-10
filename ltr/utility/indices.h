// Copyright 2011 Yandex

#ifndef LTR_UTILITY_INDICES_H_
#define LTR_UTILITY_INDICES_H_

#include <vector>

#include <boost/shared_ptr.hpp>  //NOLINT

using std::vector;

namespace ltr {
namespace utility {

typedef vector<int> Indices;
typedef boost::shared_ptr<Indices> IndicesPtr;

typedef vector<int> Permutation;

void getIdPermutation(Permutation *permution, int size);
void getRandomPermutation(Permutation *permution, int size);

void getRandomIndices(Indices *indices, int max_index, int count);
};
};

#endif  // LTR_UTILITY_INDICES_H_