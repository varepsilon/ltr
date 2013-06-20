// Copyright 2012 Yandex

#ifndef LTR_UTILITY_MACROS_H_
#define LTR_UTILITY_MACROS_H_

#include <vector>
#include <stdexcept>

using std::vector;

namespace ltr {
/**
 * Simple setter
 */
#define SET(type, name) \
  void set_##name(const type& name) { \
    name##_ = name; \
  }

/**
 * Simple getter
 */
#define GET(type, name) \
  type name() const { \
    return name##_; \
  }

/**
 * Simple getter and setter
 */
#define GET_SET(type, name) \
  GET(type, name) \
  SET(type, name)

/**
 * Simple getter for vectors
 */
#define GET_VECTOR(type, name) \
  vector<type> name() const { \
    return name##_; \
  } \
  std::size_t name##_count() const { \
    return name##_.size(); \
  } \
  const type& name(int index) const { \
    return name##_.at(index); \
  }

/**
 * Simple getter and setter for vectors
 */
#define GET_SET_VECTOR(type, name) \
  GET_VECTOR(type, name); \
  void set_##name(const vector<type>& name) { \
    name##_ = name; \
  } \
  void add_##name(const type& value) { \
    name##_.push_back(value); \
  } \
  type& name(int index) { \
    return name##_.at(index); \
  } \
  void set_##name(int index, const type &value) { \
    name##_.at(index) = value; \
  }

/**
 * Simple getter and setter for vectors of stared_ptr
 */
#define GET_SET_VECTOR_OF_PTR(type, name) \
  void set_##name(const vector<ltr::utility::shared_ptr<type> >& name) { \
    name##_ = name; \
  } \
  vector<ltr::utility::shared_ptr<type> > name() const { \
    return name##_; \
  } \
  void add_##name(const ltr::utility::shared_ptr<type> value) { \
    name##_.push_back(value); \
  } \
  int name##_count() const { \
    return name##_.size(); \
  } \
  const type& name(int index) const { \
    return *(name##_.at(index)); \
  } \
  type& name(int index) { \
    return *(name##_.at(index)); \
  } \
  ltr::utility::shared_ptr<type> name##_ptr(int index) const { \
    return name##_.at(index); \
  } \
  ltr::utility::shared_ptr<type> name##_ptr(int index) { \
    return name##_.at(index); \
  }

/**
 * Throw an exception if expression is false
 */
#define CHECK(expression) \
  if (!(expression)) { \
    throw std::logic_error("Expected: " #expression); \
  }

/**
 * Throw an exception with given message if expression is false
 */
#define CHECK_MSG(expression, message) \
  if (!(expression)) { \
    throw std::logic_error(message); \
  }

/**
 * Allows only heap creation and makes shared_ptr<A> friendly
 * Must be inserted at the beginning or end of the class
 * The only way to create object will be the following
 * \code{.cpp} bn
    Class::Ptr object(new Class());
  \endcode
 */
#define ALLOW_SHARED_PTR_ONLY_CREATION(class_name) \
public: \
  typedef ltr::utility::shared_ptr< class_name > Ptr; \
protected: \
  virtual ~class_name() {} \
  friend class ltr::utility::shared_ptr< class_name >;
}

#endif  // LTR_UTILITY_MACROS_H_
