// Copyright 2012 Yandex

#ifndef LTR_UTILITY_CONTAINER_UTILITY_H_
#define LTR_UTILITY_CONTAINER_UTILITY_H_

#include <stdexcept>
#include <sstream>

#include "boost/unordered_map.hpp"
#include "boost/unordered_set.hpp"

using std::logic_error;
using std::stringstream;

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

/**
 * Converts boost::unordered_map<Key, Value> to string
 * if the ToString(Value) is defined and operator << (ostream, Key)
 * is defined.
 * @param cont - cantainer to convert to string
 * @returns converted string
 */
template <class Key, class Value>
inline string ToString(const boost::unordered_map<Key, Value>& cont) {
  typedef typename boost::unordered_map<Key, Value>::const_iterator TIter;
  stringstream out(stringstream::out);
  size_t i = 0;
  for (TIter it = cont.begin(); it != cont.end(); ++it)
    out << ++i << ")" << it->first << " " << ToString(it->second) << '\n';
  return out.str();
}

/**
 * Converts boost::unordered_set<Value> to string
 * if the operator << (ostream, Value) is defined.
 * @param cont - cantainer to convert to string
 * @returns converted string
 */
template <class Value>
inline string ToString(const boost::unordered_set<Value>& cont) {
  typedef typename boost::unordered_set<Value>::const_iterator TIter;
  stringstream out(stringstream::out);
  out << "set(";
  for (TIter it = cont.begin(); it != cont.end(); ++it)
    out << *it << "; ";
  out << ")";
  return out.str();
}
}
}

#endif //LTR_UTILITY_CONTAINER_UTILITY_H_
