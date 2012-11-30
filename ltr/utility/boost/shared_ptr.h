// Copyright 2012 Yandex

#ifndef LTR_UTILITY_SHARED_PTR_H_
#define LTR_UTILITY_SHARED_PTR_H_

#include <algorithm>
#include <cassert>

using std::swap;

namespace ltr {
namespace utility {

template <class T>
class shared_ptr {
public:
  shared_ptr() : ptr(NULL), count(nil_counter()) {
    incref();
  }

  ~shared_ptr() {
    decref();
  }

  template<class Y>
  shared_ptr(const shared_ptr<Y>& rhs): ptr(rhs.get()), count(rhs.get_count()) {
    incref();
  }

  shared_ptr(const shared_ptr<T>& rhs): ptr(rhs.get()), count(rhs.get_count()) {
    incref();
  }

  shared_ptr(T* rhs) {
     this->ptr = rhs;
     this->count = new unsigned(1);
  }

  shared_ptr<T>& operator=(const shared_ptr<T>& rhs) {
    if (this->get() == rhs.get()) {
      return *this;
    }
    decref();
    this->ptr = rhs.get();
    this->count = rhs.get_count();
    incref();
    return *this;
  }

  unsigned* get_count() const {
    return count;
  }
  T* get() const {
    return ptr;
  }
  T* operator->() const {
    return ptr;
  }
  T& operator*() const {
    return *ptr;
  }

  operator bool() const {
    return ptr != NULL;
  }

  bool operator<(const shared_ptr<T>& rhs) const {
    return get() < rhs.get();
  }

  unsigned refcount() const {
    return *count;
  }

  void swap(shared_ptr<T>& rhs) {
    std::swap(this->ptr, rhs.ptr);
    std::swap(this->count, rhs.count);
  }

  void reset() {
    shared_ptr<T>().swap(*this);
  }

  template<class Y>
  void reset(Y* rhs) {
    assert(rhs != 0 && rhs != this->ptr);  // catch self-reset errors
    shared_ptr<T>(rhs).swap(*this);
  }
private:
  T* ptr;
  unsigned* count;

  // special case, null pointer (nil-code)
  static unsigned* nil_counter() {
    static unsigned nil_counter_ = 1;
    return &nil_counter_;
  }

  void decref() {
    if (--(*(this->count)) == 0) {
      delete this->ptr;
      delete this->count;
    }
  }
  void incref() {
    ++(*(this->count));
  }
};

}
}

#endif  // LTR_UTILITY_SHARED_PTR_H_
