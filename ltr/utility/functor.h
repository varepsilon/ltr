// Copyright 2012 Yandex

#ifndef LTR_UTILITY_FUNCTOR_H_
#define LTR_UTILITY_FUNCTOR_H_

#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

#include <string>

#include "ltr/data/object.h"

namespace ltr {
/** Base class for all functors on Object.
 * \tparam TValue - type of the result of the function on Object
 */
template<class TValue>
class Functor {
  public:
  /** \typedef boost shared pointer to the Functor
   */
  typedef boost::shared_ptr<Functor> Ptr;
  /**
   * Function returns the value of this functor on the given object
   */
  TValue operator()(const Object& obj) const {
    return value(obj);
  }
  /**
   * Function returns the value of this functor for the given object
   */
  virtual TValue value(const Object& obj) const = 0;

  virtual ~Functor() {}
};
}
#endif  // LTR_UTILITY_FUNCTOR_H_
