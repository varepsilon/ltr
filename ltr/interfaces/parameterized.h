// Copyright 2011 Yandex

#ifndef LTR_INTERFACES_PARAMETERIZED_H_
#define LTR_INTERFACES_PARAMETERIZED_H_

#include <stdexcept>
#include <string>
#include <vector>

#include "ltr/parameters_container/parameters_container.h"

using std::logic_error;

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
  std::vector<type> name() const { \
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
  void set_##name(const std::vector<type>& name) { \
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
  void set_##name(const std::vector<ltr::utility::shared_ptr<type> >& name) { \
    name##_ = name; \
  } \
  std::vector<ltr::utility::shared_ptr<type> > name() const { \
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
    throw logic_error("Expected: " #expression); \
  }

/**
 * Parameterized is a class that provides derived subclasses hold their
 * different-type parameters (int, double, bool, etc.) in convenient
 * way. E.g. a derived subclass knows it's default parameters
 */
class Parameterized {
 public:
  virtual ~Parameterized();
  /**
   * Set default values of parameters
   */
  virtual void setDefaultParameters();
  /**
   * Perform checks of parameters. In case of wrong parameters
   * std::logic_error will be thrown.
   */
  virtual void checkParameters() const;

  void setParameters(const ParametersContainer &parameters);

 protected:
  virtual void setParametersImpl(const ParametersContainer& parameters);
};
};
#endif  // LTR_INTERFACES_PARAMETERIZED_H_
