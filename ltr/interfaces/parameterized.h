// Copyright 2011 Yandex

#ifndef LTR_INTERFACES_PARAMETERIZED_H_
#define LTR_INTERFACES_PARAMETERIZED_H_

#include <string>
#include <set>
#include <vector>

#include "ltr/parameters_container/parameters_container.h"

using std::string;
using std::set;
using std::vector;

#define CHECK_PARAMETER(type, name, condition) \
  { \
    type X = (static_cast<const Parameterized* const>(this))\
    ->getParameter<type>(name); \
    if (!(condition)) \
      throw std::logic_error("Error in parameter '" \
        name "' check: " #condition); \
  }

#define CHECK_INT_PARAMETER(name, condition) \
  CHECK_PARAMETER(int, name, condition)
#define CHECK_DOUBLE_PARAMETER(name, condition) \
  CHECK_PARAMETER(double, name, condition)
#define CHECK_BOOL_PARAMETER(name, condition) \
  CHECK_PARAMETER(bool, name, condition)
#define CHECK_LIST_PARAMETER(name, condition) \
  CHECK_PARAMETER(vector<int>, name, condition)

#define CHECK_HAS_GROUP(group) \
  if (!parameters_.hasGroup(group)) \
    throw std::logic_error("Class must has '" #group "' parameter group");

#define CHECK_HAS_NO_GROUP(group) \
  if (parameters_.hasGroup(group)) \
    throw std::logic_error("Class must has no '" #group "' parameter group");

#define CHECK_HAS_PARAMETER(type, name, group) \
  if (!parameters_.has<type>(name, group)) \
    throw std::logic_error("Class must has " #type " parameter " #name);

#define CHECK_HAS_NO_PARAMETER(type, name, group) \
  if (parameters_.has<type>(name, group)) \
    throw std::logic_error("Class mustn't has " #type " parameter " #name);

namespace ltr {
/**
 * Parameterized is a class that provides derived subclasses hold their
 * different-type parameters (int, double and bool) in convenient
 * way. E.g. a derived subclass knows it's default parameters
 */
class Parameterized {
  protected:
  /** Container, which contain all the parameters of this object.
   */
  ParametersContainer parameters_;
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
  /**
   * This function is called after updation of any parameter.
   * By default does nothing.
   */
  virtual void updateParameters() {}

  /**
   * Returns parametersContainer of this parametrized object. 
   */
  const ParametersContainer& parameters() {
    return parameters_;
  }
  /**
   * Adds new parameter. Checks, that there were no such parameter yet.
   */
  template<class T>
  void addParameter(const string& name, T value, string group="") {
    CHECK_HAS_NO_PARAMETER(T, name, group);
    parameters_.set<T>(name, value, group);
  }
  /**
   * Sets value of the parameter.
   * Checks, that there was such a parameter.
   */
  template<class T>
  void setParameter(const string& name, T value, string group="") {
    CHECK_HAS_PARAMETER(T, name, group);
    parameters_.set<T>(name, value, group);
    checkParameters();
    updateParameters();
  }
  /** Sets integer parameter and calls checkParameters() and updateParameters()
   */
  void setIntParameter(const string& name, int value, string group="") {
    setParameter<int>(name, value, group);
  }
  /** Sets double parameter and calls checkParameters() and updateParameters()
   */
  void setDoubleParameter(const string& name, double value, string group="") {
    setParameter<double>(name, value, group);
  }
  /** Sets bool parameter and calls checkParameters() and updateParameters()
   */
  void setBoolParameter(const string& name, bool value, string group="") {
    setParameter<bool>(name, value, group);
  }
  /** Sets list(array of integers) parameter and calls checkParameters()
   * and updateParameters()
   */
  void setListParameter(const string& name,
                        vector<int> value,
                        string group="") {
    setParameter<vector<int> >(name, value, group);
  }

  /** Adds new integer parameter.
   */
  void addIntParameter(const string& name, int value, string group="") {
    addParameter<int>(name, value, group);
  }
  /** Adds new double parameter.
   */
  void addDoubleParameter(const string& name, double value, string group="") {
    addParameter<double>(name, value, group);
  }
  /** Adds new bool parameter.
   */
  void addBoolParameter(const string& name, bool value, string group="") {
    addParameter<bool>(name, value, group);
  }
  /** Adds new list(array of integers) parameter.
   */
  void addListParameter(const string& name,
                        vector<int> value,
                        string group="") {
    addParameter<vector<int> >(name, value, group);
  }

  /** Copies parameters from another parameter container
   * @param params - container to copy parameters from.
   */
  void copyParameters(const ParametersContainer& params) {
    parameters_.copy(params);
    checkParameters();
    updateParameters();
  }
  /**
   * Removes all the parameters.
   */
  void clearParameters() {
    parameters_.clear();
  }

  /** Returns the value of parameter with name 'name'
   */
  template<class T>
  T getParameter(const string& name, string group="") const {
    return parameters_.get<T>(name, group);
  }
  /** Returns the value of integer parameter with name 'name'
   */
  int getIntParameter(const string& name, string group="") const {
    return parameters_.getInt(name, group);
  }
  /** Returns the value of double parameter with name 'name'
   */
  double getDoubleParameter(const string& name, string group="") const {
    return parameters_.getDouble(name, group);
  }
  /** Returns the value of bool parameter with name 'name'
   */
  bool getBoolParameter(const string& name, string group="") const {
    return parameters_.getBool(name, group);
  }
  /** Returns the value of list parameter with name 'name'
   */
  vector<int> getListParameter(const string& name, string group="") const {
    return parameters_.getList(name, group);
  }

  /**
   * Removes group of parameters.
   */
  void removeParametersGroup(const string& group) {
    parameters_.removeGroup(group);
  }
  /**
   * Adds all the parameters from the given parametersContainer
   * into the given group.
   * @param container - container to copy from.
   * @param group - group to copy parameters in.
   */
  void addParametersGroup(const ParametersContainer& container,
                          const string& group) {
    parameters_.setGroup(container, group);
  }
  virtual ~Parameterized() {}
};
};

#endif  // LTR_INTERFACES_PARAMETERIZED_H_
