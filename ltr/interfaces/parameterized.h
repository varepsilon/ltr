// Copyright 2011 Yandex

#ifndef LTR_INTERFACES_PARAMETERIZED_H_
#define LTR_INTERFACES_PARAMETERIZED_H_

#include <stdexcept>
#include <string>

#include "ltr/parameters_container/parameters_container.h"

using std::logic_error;

namespace ltr {

/**
* Simple getter and setter
*/
#define GET_SET(type, name) \
  void set_##name(const type& name) { \
    name##_ = name; \
  }; \
  type name() const { \
    return name##_; \
  };

/**
* Simple setter
*/
#define SET(type, name) \
  void set_##name(const type& name) { \
    name##_ = name; \
  };

/**
* Simple getter
*/
#define GET(type, name) \
  type name() const { \
    return name##_; \
  };

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
