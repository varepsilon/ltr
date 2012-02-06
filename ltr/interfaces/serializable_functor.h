// Copyright 2012 Yandex

#ifndef LTR_UTILITY_SERIALIZABLE_FUNCTOR_H_
#define LTR_UTILITY_SERIALIZABLE_FUNCTOR_H_

#include <boost/shared_ptr.hpp>

#include "ltr/interfaces/functor.h"
#include "ltr/interfaces/serializable.h"

namespace ltr {

template <typename TValue>
class SerializableFunctor : public Functor<TValue>, public Serializable {
  public:
  typedef boost::shared_ptr<SerializableFunctor> Ptr;
  virtual ~SerializableFunctor() {}
};
}
#endif  // LTR_UTILITY_SERIALIZABLE_FUNCTOR_H_
