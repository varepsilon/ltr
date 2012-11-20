// Copyright 2012 Yandex

#ifndef LTR_DATA_PER_OBJECT_ACCESSOR_H_
#define LTR_DATA_PER_OBJECT_ACCESSOR_H_

#include "ltr/data/object.h"
#include "ltr/data/object_pair.h"
#include "ltr/data/object_list.h"

#include "ltr/utility/macros.h"

#include "ltr/utility/boost/shared_ptr.h"

using ltr::Object;
using ltr::ObjectPair;
using ltr::ObjectList;

namespace ltr {

template<class TElement>
class PerObjectAccessor {
 public:
  explicit PerObjectAccessor(TElement* element) : element_(element) {}

  int object_count() const;

  Object& object(int object_index);

 private:
  TElement* element_;
};

// template realization

template<>
class PerObjectAccessor<Object> {
 public:
  explicit PerObjectAccessor(Object* element) : element_(element) {}

  int object_count() const {
    return 1;
  }

  Object& object(int object_index) {
    CHECK(object_index < object_count());
    return *element_;
  }

 private:
  Object* element_;
};

template<>
class PerObjectAccessor<const Object> {
 public:
  explicit PerObjectAccessor(const Object* element) : element_(element) {}

  int object_count() const {
    return 1;
  }

  const Object& object(int object_index) {
    CHECK(object_index < object_count());
    return *element_;
  }

 private:
  const Object* element_;
};


template<>
class PerObjectAccessor<ObjectPair> {
 public:
  explicit PerObjectAccessor(ObjectPair* element) : element_(element) {}

  int object_count() const {
    return 2;
  }

  Object& object(int object_index) {
    CHECK(object_index < object_count());
    if (object_index == 0) {
      return element_->first;
    } else if (object_index == 1) {
      return element_->second;
    }
  }

 private:
  ObjectPair* element_;
};

template<>
class PerObjectAccessor<const ObjectPair> {
 public:
  explicit PerObjectAccessor(const ObjectPair* element) : element_(element) {}

  int object_count() const {
    return 2;
  }

  const Object& object(int object_index) {
    CHECK(object_index < object_count());
    if (object_index == 0) {
      return element_->first;
    } else if (object_index == 1) {
      return element_->second;
    }
  }

 private:
  const ObjectPair* element_;
};

template<>
class PerObjectAccessor<ObjectList> {
 public:
  explicit PerObjectAccessor(ObjectList* element) : element_(element) {}

  int object_count() const {
    return element_->size();
  }

  Object& object(int object_index) {
    CHECK(object_index < object_count());
    return element_->at(object_index);
  }

 private:
  ObjectList* element_;
};

template<>
class PerObjectAccessor<const ObjectList> {
 public:
  explicit PerObjectAccessor(const ObjectList* element) : element_(element) {}

  int object_count() const {
    return element_->size();
  }

  const Object& object(int object_index) {
    CHECK(object_index < object_count());
    return element_->at(object_index);
  }

 private:
  const ObjectList* element_;
};
}

#endif  // LTR_DATA_PER_OBJECT_ACCESSOR_H_
