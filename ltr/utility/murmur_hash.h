// Copyright 2012 Yandex

#ifndef LTR_UTILITY_MURMUR_HASH_H_
#define LTR_UTILITY_MURMUR_HASH_H_

namespace ltr {
namespace utility {

unsigned int murmurHash(const void* key, int len, unsigned int seed);
}
}

#endif  // LTR_UTILITY_MURMUR_HASH_H_
