// Copyright Yandex 2012
#ifndef LTR_CLIENT_UTILITY_COMMON_UTILITY_H_
#define LTR_CLIENT_UTILITY_COMMON_UTILITY_H_

#include <map>

/**
 * Deletes all the data from the map.
 * @param mapInstance - std::map pointer to be cleared.
 */

using std::map;

template <class Key, class Value>
static inline void DeleteAllFromMap(map<Key, Value>* mapInstance) {
  assert(mapInstance);
  // C++11 We have no unique_ptr so we MUST delete pointers manually
  for (typename map<Key, Value>::const_iterator it =
       mapInstance->begin();
       it != mapInstance->end();
       ++it) {
    delete it->second;
  }
}

#endif //LTR_CLIENT_UTILITY_COMMON_UTILITY_H_

