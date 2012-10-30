// Copyright Yandex 2012
#ifndef LTR_CLIENT_UTILITY_COMMON_UTILITY_H_
#define LTR_CLIENT_UTILITY_COMMON_UTILITY_H_

/**
 * Deletes all the data from the boost::unordered_map.
 * @param unorderedMapInstance - the boost::unordered_map
 * pointer to be cleared.
 */

template <class Key, class Value>
static inline void DeleteAllFromUnorderedMap(
    boost::unordered_map<Key, Value>* unorderedMapInstance
    ) {
  assert(unorderedMapInstance);
  // C++11 We have no unique_ptr so we MUST delete pointers manually
  for (typename boost::unordered_map<Key, Value>::const_iterator it =
       unorderedMapInstance->begin();
       it != unorderedMapInstance->end();
       ++it) {
    delete it->second;
  }
}

#endif //LTR_CLIENT_UTILITY_COMMON_UTILITY_H_

