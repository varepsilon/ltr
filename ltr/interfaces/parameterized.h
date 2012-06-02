// Copyright 2011 Yandex

#ifndef LTR_INTERFACES_PARAMETERIZED_H_
#define LTR_INTERFACES_PARAMETERIZED_H_

#include <stdexcept>
#include <string>

#include "ltr/parameters_container/parameters_container.h"

using std::logic_error;

namespace ltr {

#define GET_SET(type, name) \
  void set_##name(type name) { \
    name##_ = name; \
  }; \
  type name() { \
    return name##_; \
  };
#define SET(type, name) \
  void set_##name(type name) { \
    name##_ = name; \
  };
#define GET(type, name) \
  type name() { \
    return name##_; \
  };

#define CHECK(expr) \
  if (!(expr)) { \
    throw logic_error("Expected: " #expr); \
  }


class Parameterized {
 public:
  virtual ~Parameterized();
  virtual void setDefaultParameters();
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
  // \deprecated
  void copyParameters(const ParametersContainer &parameters);
 protected:
  virtual void setParametersImpl(const ParametersContainer &parameters);
  virtual void parametersUpdateCallback();
 private:
  // \deprecated
  ParametersContainer parameters_;
};
};

#endif  // LTR_INTERFACES_PARAMETERIZED_H_
