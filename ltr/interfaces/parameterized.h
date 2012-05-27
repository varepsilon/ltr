// Copyright 2011 Yandex

#ifndef LTR_INTERFACES_PARAMETERIZED_H_
#define LTR_INTERFACES_PARAMETERIZED_H_

#include <string>
#include <stdexcept>

#include "ltr/parameters_container/parameters_container.h"

using std::logic_error;

namespace ltr {

#define MAKE_GET_SET(type, name) \
  type name() { \
    return name##_; \
  }; \
  void set_##name(type name) { \
    name##_ = name; \
    checkParameters(); \
  };

#define MAKE_SET(type, name) \
  void set_##name(type name) { \
    name##_ = name; \
    checkParameters(); \
  };

#define MAKE_GET(type, name) \
  type name() { \
    return name##_; \
  };

#define CHECK(expr) \
  if (!(expr)) { \
    throw logic_error("Wrong parameter: need " #expr); \
  }


class Parameterized {
 public:
  virtual ~Parameterized();
  virtual void setDefaultParameters();
  virtual void checkParameters() const;
  void setParameters(const ParametersContainer &parameters);
  // \depricated
  template <class T, class TPred>
  void checkParameter(const std::string &name, TPred pred) const
      throw(std::logic_error, std::bad_cast) {
    const T &value = parameters_.Get<T>(name);
    if (!pred(value)) {
      throw std::logic_error("Error in parameter " + name + " check");
    }
  }
  // \depricated
  template<class T>
  void setExistingParameter(const std::string &name, T value) {
    parameters_.SetExisting(name, value);
    checkParameters();
    parametersUpdateCallback();
  }
  // \depricated
  void clearParameters() {
    parameters_.Clear();
  }
  // \depricated
  template<class T>
  void addNewParam(const std::string &name, const T &value) {
    parameters_.AddNew(name, value);
  }
  // \depricated
  const ParametersContainer& parameters() const;
  // \depricated
  void copyParameters(const ParametersContainer &parameters);
 protected:
  virtual void setParametersImpl(const ParametersContainer &parameters);
  virtual void parametersUpdateCallback();
 private:
  // \depricated
  ParametersContainer parameters_;
};
};

#endif  // LTR_INTERFACES_PARAMETERIZED_H_
