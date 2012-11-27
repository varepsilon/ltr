// Copyright 2011 Yandex

#ifndef LTR_INTERFACES_PARAMETERIZED_H_
#define LTR_INTERFACES_PARAMETERIZED_H_

#include "ltr/parameters_container/parameters_container.h"

namespace ltr {
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
