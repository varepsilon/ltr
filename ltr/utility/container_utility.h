// Copyright 2012 Yandex

#ifndef LTR_UTILITY_CONTAINER_UTILITY_H_
#define LTR_UTILITY_CONTAINER_UTILITY_H_

#include <stdexcept>

using std::logic_error;

namespace ltr {
namespace utility {
template <class TCont>
static inline bool Contains(const TCont& cont,
                            const typename TCont::key_type& key) {
  return cont.find(key) != cont.end();
}

template <class ValueType>
static inline ValueType &SafeInsert(
    boost::unordered_map<string, ValueType> &container,
    const char* key) {
  if (!key) {
    throw logic_error("empty name!!!!");
  }
  if (Contains(container, key)) {
    throw logic_error("Container already contains " + string(key) + "!!!");
  }
  return container[key];
}

static inline void SafeInsert(
    boost::unordered_set<string>& cont,
    const char* key) {
  if (!key) {
    return;
  }
  if (Contains(cont, key)) {
    throw logic_error("Container already contains " + string(key) + "!!!");
  }
  cont.insert(key);
}
}
}

#endif //LTR_UTILITY_CONTAINER_UTILITY_H_
