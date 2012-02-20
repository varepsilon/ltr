// Copyright 2011 Yandex

#ifndef LTR_INTERFACES_PARAMETERIZED_H_
#define LTR_INTERFACES_PARAMETERIZED_H_

#include <string>
#include <vector>

#include "ltr/parameters_container/parameters_container.h"

using std::string;

namespace ltr {
/**
 * Parameterized is a class that provides derived subclasses hold their
 * different-type parameters (int, double and bool) in convenient
 * way. E.g. a derived subclass knows it's default parameters
 */
class Parameterized {
  protected:
  Parameterized() {}

  public:
  /**
   * By default simply clears all parameters
   */
  virtual void setDefaultParameters();
  /**
   * checks, if current parameters are valid.
   * If invalid parameters found - logic_error is thrown.
   */
  virtual void checkParameters() const;
  /** Sets integer parameter and calls checkParameters()
   */
  void setIntParameter(const string& name, int value) {
    parameters_.setInt(name, value);
    checkParameters();
  }
  /** Sets double parameter and calls checkParameters()
   */
  void setDoubleParameter(const string& name, double value) {
    parameters_.setDouble(name, value);
    checkParameters();
  }
  /** Sets bool parameter and calls checkParameters()
   */
  void setBoolParameter(const string& name, bool value) {
    parameters_.setBool(name, value);
    checkParameters();
  }
  /** Sets list(array of integers) parameter and calls checkParameters()
   */
  void setListParameter(const string& name, vector<int> value) {
    parameters_.setList(name, value);
    checkParameters();
  }

  /** Sets integer parameter, but don't calls checkParameters()
   */
  void addIntParameter(const string& name, int value) {
    parameters_.setInt(name, value);
  }
  /** Sets double parameter, but don't calls checkParameters()
   */
  void addDoubleParameter(const string& name, double value) {
    parameters_.setDouble(name, value);
  }
  /** Sets bool parameter, but don't calls checkParameters()
   */
  void addBoolParameter(const string& name, bool value) {
    parameters_.setBool(name, value);
  }
  /** Sets list(array of integers) parameter, but don't calls checkParameters()
   */
  void addListParameter(const string& name, vector<int> value) {
    parameters_.setList(name, value);
  }

  /** Copies parameters from another parameter container
   * @param params - container to copy parameters from.
   */
  void copyParameters(const ParametersContainer& params) {
    parameters_.copyParameters(params);
    checkParameters();
  }
  /**
   * Removes all the parameters.
   */
  void clearParameters() {
    parameters_.clear();
  }
  /** Returns the value of integer parameter with name 'name'
   */
  int getIntParameter(const string& name) const {
    return parameters_.getInt(name);
  }
  /** Returns the value of double parameter with name 'name'
   */
  double getDoubleParameter(const string& name) const {
    return parameters_.getDouble(name);
  }
  /** Returns the value of bool parameter with name 'name'
   */
  bool getBoolParameter(const string& name) const {
    return parameters_.getBool(name);
  }
  /** Returns the value of list parameter with name 'name'
   */
  vector<int> getListParameter(const string& name) const {
    return parameters_.getList(name);
  }
  virtual ~Parameterized() {}
  protected:
  /** Container, which contain all the parameters of this object.
   */
  ParametersContainer parameters_;
};
};

#endif  // LTR_INTERFACES_PARAMETERIZED_H_
