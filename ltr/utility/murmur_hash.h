// Copyright 2012 Yandex

#ifndef LTR_UTILITY_MURMUR_HASH_H_
#define LTR_UTILITY_MURMUR_HASH_H_

#include <string>
using std::string;

namespace ltr {
namespace utility {

/**
 * MurmurHash, by Austin Appleby
 * Note - This code makes a few assumptions about how your machine behaves -
 * 1. We can read a 4-byte value from any address without crashing
 * 2. sizeof(int) == 4
 * And it has a few limitations -
 * 1. It will not work incrementally.
 * 2. It will not produce the same results on little-endian and big-endian
 *    machines.
*/
unsigned int murmurHash(const void* key, int len, unsigned int seed);
unsigned int hash(const string& str);
}
}

#endif  // LTR_UTILITY_MURMUR_HASH_H_
