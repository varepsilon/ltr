// Copyright 2012 Yandex

#ifndef LTR_UTILITY_CONTAINER_UTILITY_H_
#define LTR_UTILITY_CONTAINER_UTILITY_H_

#include <stdexcept>
#include <sstream>
#include <map>
#include <set>
#include <list>

using std::logic_error;
using std::stringstream;
using std::list;
using std::map;


namespace ltr {
namespace utility {
template <class TCont>
static inline bool Contains(const TCont& cont,
                            const typename TCont::key_type& key) {
  return cont.find(key) != cont.end();
}

template <class ValueType>
static inline ValueType &SafeInsert(
    std::map<string, ValueType> &container,
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
    std::set<string>& cont,
    const char* key) {
  if (!key) {
    return;
  }
  if (Contains(cont, key)) {
    throw logic_error("Container already contains " + string(key) + "!!!");
  }
  cont.insert(key);
}

/**
 * Converts std::map<Key, Value> to string
 * if the ToString(Value) is defined and operator << (ostream, Key)
 * is defined.
 * @param cont - container to convert to string
 * @returns converted string
 */
template <class Key, class Value>
inline string ToString(const std::map<Key, Value>& cont) {
  typedef typename std::map<Key, Value>::const_iterator TIter;
  stringstream out(stringstream::out);
  size_t i = 0;
  for (TIter it = cont.begin(); it != cont.end(); ++it)
    out << ++i << ")" << it->first << " " << ToString(it->second) << '\n';
  return out.str();
}

/**
 * Converts std::set<Value> to string
 * if the operator << (ostream, Value) is defined.
 * @param cont - container to convert to string
 * @returns converted string
 */
template <class Value>
inline string ToString(const std::set<Value>& cont) {
  typedef typename std::set<Value>::const_iterator TIter;
  stringstream out(stringstream::out);
  out << "set(";
  for (TIter it = cont.begin(); it != cont.end(); ++it)
    out << *it << "; ";
  out << ")";
  return out.str();
}

/**
 * Extracts values from std::map<Key, Value>
 * @param dictionary - std::map<Key, Value>
 * @returns list of pointers to values
 */
template <typename Key, typename Value>
list<const Value*> GetValues(const map<Key, Value>& dictionary) {
  list<const Value*> result;
  for (typename map<Key, Value>::const_iterator it = dictionary.begin();
       it != dictionary.end();
       ++it) {
    const Value& value = it->second;
    result.push_back(&value);
  }
  return result;
}

/**
 * Extracts keys from std::map<Key, Value>
 * @param dictionary - std::map<Key, Value>
 * @returns list of pointers to keys
 */
template <typename Key, typename Value>
list<const Key*> GetKeys(const map<Key, Value>& dictionary) {
  list<const Key*> result;
  for (typename map<Key, Value>::const_iterator it = dictionary.begin();
       it != dictionary.end();
       ++it) {
    const Key& key = it->first;
    result.push_back(&key);
  }
  return result;
}
}
}

#endif //LTR_UTILITY_CONTAINER_UTILITY_H_
