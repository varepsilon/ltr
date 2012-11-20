// Copyright 2012 Yandex

#ifndef LTR_INTERFACES_SERIALIZABLE_FUNCTOR_H_
#define LTR_INTERFACES_SERIALIZABLE_FUNCTOR_H_

#include "ltr/utility/boost/shared_ptr.h"

#include "ltr/interfaces/functor.h"
#include "ltr/interfaces/serializable.h"

namespace ltr {
template <typename TValue>
class SerializableFunctor : public Functor<TValue>, public Serializable {
  public:
  typedef ltr::utility::shared_ptr<SerializableFunctor> Ptr;
  virtual ~SerializableFunctor() {}
};
}
#endif  // LTR_INTERFACES_SERIALIZABLE_FUNCTOR_H_
