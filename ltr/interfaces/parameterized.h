// Copyright 2011 Yandex

#ifndef LTR_INTERFACES_PARAMETERIZED_H_
#define LTR_INTERFACES_PARAMETERIZED_H_

#include <stdexcept>
#include <string>

#include "ltr/parameters_container/parameters_container.h"

using std::logic_error;

namespace ltr {

#define GET_SET(type, name) \
  void set_##name(const type &name) { \
    name##_ = name; \
  }; \
  type name() const { \
    return name##_; \
  };
#define SET(type, name) \
  void set_##name(const type &name) { \
    name##_ = name; \
  };
#define GET(type, name) \
  type name() const { \
    return name##_; \
  };

#define CHECK(expression) \
  if (!(expression)) { \
    throw logic_error("Expected: " #expression); \
  }

/**
 * Parameterized is a class that provides derived subclasses hold their
 * different-type parameters (int, double and bool) in convenient
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
  // \deprecated
  template <class T, class TPred>
  void checkParameter(const std::string &name, TPred pred) const
      throw(std::logic_error, std::bad_cast) {
    const T &value = parameters_.Get<T>(name);
    if (!pred(value)) {
      throw std::logic_error("Error in parameter " + name + " check");
    }
  }
  // \deprecated
  template<class T>
  void setExistingParameter(const std::string &name, T value) {
    parameters_.SetExisting(name, value);
    checkParameters();
    parametersUpdateCallback();
  }
  // \deprecated
  void clearParameters() {
    parameters_.Clear();
  }
  // \deprecated
  template<class T>
  void addNewParam(const std::string &name, const T &value) {
    parameters_.AddNew(name, value);
  }
  // \deprecated
  const ParametersContainer& parameters() const;
  template <typename DesiredType>
  DesiredType getParameter(const string& name) const;
  // \deprecated
  void copyParameters(const ParametersContainer &parameters);
 protected:
  virtual void setParametersImpl(const ParametersContainer &parameters);
  virtual void parametersUpdateCallback();
 private:
  // \deprecated
  ParametersContainer parameters_;
};

// template realization

template <class DesiredType>
DesiredType Parameterized::getParameter(const string& name) const {
  try {
    return parameters_.Get<DesiredType>(name);
  } catch (std::logic_error error) {
    try {
      return parameters_.Get<Parameterized*, DesiredType>(name);
    } catch (std::logic_error error) {
      throw;
    }
  }
}

template<>
inline double Parameterized::getParameter<double>(const string& name) const {
  return parameters_.Get<double>(name);
}

template<>
inline int Parameterized::getParameter<int>(const string& name) const {
  return parameters_.Get<int>(name);
}

template<>
inline bool Parameterized::getParameter<bool>(const string& name) const {
  return parameters_.Get<bool>(name);
}

template<>
inline float Parameterized::getParameter<float>(const string& name) const {
  return parameters_.Get<float>(name);
}

};
#endif  // LTR_INTERFACES_PARAMETERIZED_H_
