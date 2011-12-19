// Copyright 2011 Yandex

#ifndef LTR_INTERFACES_PARAMETERIZED_H_
#define LTR_INTERFACES_PARAMETERIZED_H_

#include <string>

#include "ltr/parameters_container/parameters_container.h"

using std::string;

namespace ltr {
  /**
   * Parameterized is a class that provides derived subclasses hold their
   * different-type parameters (int, double and bool) in convenient
   * way. E.g. a derived subclass knows it's default parameters
   */
  class Parameterized {
  public:
    ParametersContainer& parameters();
    const ParametersContainer& parameters() const;
    /**
     * By default simply clears all parameters
     */
    virtual void setDefaultParameters();
     /**
      * checks, if current parameters are valid.
      * If invalid parameters found - description is written in result. 
      * If no invalids - result will be empty
      * Always return empty (ok) by default
      */
    virtual std::string checkParameters() const;
    virtual ~Parameterized() {}
  protected:
    ParametersContainer parameters_;
  };
};

#endif  // LTR_INTERFACES_PARAMETERIZED_H_
