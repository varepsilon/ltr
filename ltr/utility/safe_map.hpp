// Copyright 2013 Yandex

#ifndef LTR_UTILITY_SAFE_MAP_HPP_
#define LTR_UTILITY_SAFE_MAP_HPP_

#include <map>
#include <sstream>
#include <string>

#include "ltr/interfaces/printable.h"
#include "ltr/utility/macros.h"
#include "ltr/utility/boost/lexical_cast.hpp"

using std::stringstream;
using std::string;
using std::map;

using ltr::Printable;
using ltr::utility::lexical_cast;

namespace ltr {
namespace utility {
/**
 * \brief std::map with some safe methods.
 *
 * \tparam Key - type of the keys.
 * \tparam T - type of the mapped value.
 */
template <class Key, class T>
class SafeMap: public map<Key, T>, public Printable {
 public:
  virtual ~SafeMap() { }
  /**
   * Searches the container for an element with an equivalent key and returns an
   * iterator to it if found, otherwise it throws exception.
   *
   * \param key - key to be searched for
   * \return - an iterator to the element
   */
  typename map<Key, T>::iterator safeFind(const Key& key) {
    typename map<Key, T>::iterator it = this->find(key);
    CHECK_MSG(it != this->end(),
              "map::safeFind: object with key \"" + lexical_cast<string>(key) +
              "\" not found");
    return it;
  }

  /**
   * Searches the container for an element with an equivalent key and returns an
   * iterator to it if found, otherwise it throws exception.
   *
   * \param key - key to be searched for
   * \return - an iterator to the element
   */
  typename map<Key, T>::const_iterator safeFind(const Key& key) const {
    typename map<Key, T>::const_iterator it = this->find(key);
    CHECK_MSG(it != this->end(),
      "map::safeFind: object with given key not found");
    return it;
  }

  /**
   * Returns true if map contains key.
   *
   * \param key - key to be searched for
   */
  bool contains(const Key& key) const {
    return (this->find(key) != this->end());
  }

  /**
   * Inserts given key to map and returns reference to corresponding value if
   * there are no elements with equal key or throws exception otherwise.
   *
   * \param key - key to be inserted
   * \return - reference to corresponding value
   */
  T& safeInsert(const Key& key) {
    CHECK_MSG(!contains(key), "map::safeInsert: map already contains this key");
    return (*this)[key];
  }

  string toString() const {
    stringstream out(stringstream::out);
    int element_number = 0;
    for (typename map<Key, T>::const_iterator it = this->begin();
         it != this->end();
         ++it)
      out << ++element_number << ")" << lexical_cast<string>(it->first) << " "
          << lexical_cast<string>(it->second) << '\n';
    return out.str();
  }

  /**
   * Returns constant link to value with given key, if it exists, or throws
   * error otherwise.
   *
   * \param key - key to be searched for.
   * \return Link to value.
   */
  const T& safeAt(const Key& key) const {
    return safeFind(key)->second;
  }

  /**
   * Returns link to value with given key, if it exists, or throws
   * error otherwise.
   *
   * \param key - key to be searched for.
   * \return Link to value.
   */
  T& safeAt(const Key& key) {
    return safeFind(key)->second;
  }
};
};
};
#endif  // LTR_UTILITY_SAFE_MAP_HPP_
