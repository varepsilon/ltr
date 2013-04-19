// Copyright 2012 Yandex

#ifndef LTR_INTERFACES_FUNCTOR_H_
#define LTR_INTERFACES_FUNCTOR_H_

#include "ltr/utility/boost/shared_ptr.h"
#include "ltr/utility/boost/lexical_cast.h"

#include <string>

#include "ltr/data/object.h"

namespace ltr {
/**
 * \brief Base class for all functors on Object.
 *
 * \tparam TValue is a type of the result of the function on Object
 */
template<class TValue>
class Functor {
  public:
  /**
   * boost shared pointer to the Functor
   */
  typedef ltr::utility::shared_ptr<Functor> Ptr;
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
#endif  // LTR_INTERFACES_FUNCTOR_H_
