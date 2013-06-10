// Copyright 2013 Yandex

#ifndef LTR_UTILITY_SAFE_SET_HPP_
#define LTR_UTILITY_SAFE_SET_HPP_

#include <set>
#include <sstream>
#include <string>

#include "ltr/interfaces/printable.h"
#include "ltr/utility/macros.h"
#include "ltr/utility/boost/lexical_cast.hpp"

using std::set;
using std::stringstream;
using std::string;

using ltr::Printable;
using ltr::utility::lexical_cast;

namespace ltr {
namespace utility {
/**
 * \brief Set with some safe methods.
 *
 * \tparam T - type of the elements.
 */
template <class T>
class SafeSet: public set<T>, public Printable {
 public:
  /**
   * Returns true if set contains given key
   *
   * \param key - key to be searched for
   */
  bool contains(const T& key) const {
    return (this->find(key) != this->end());
  }

  /**
   * Safely inserts given key to set
   *
   * \param key - key to be inserted
   */
  void safeInsert(const T& key) {
    CHECK_MSG(!contains(key), "set::safeInsert: set already contains this key");
    this->insert(key);
  }

  string toString() const {
    stringstream out(stringstream::out);
    out << "set(";
    for (typename std::set<T>::const_iterator it = this->begin();
         it != this->end();
         ++it) {
      out << lexical_cast<string>(*it) << "; ";
    }
    out << ")";
    return out.str();
  }
};
};
};
#endif  // LTR_UTILITY_SAFE_SET_HPP_
