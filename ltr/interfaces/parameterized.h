// Copyright 2011 Yandex

#ifndef LTR_INTERFACES_PARAMETERIZED_H_
#define LTR_INTERFACES_PARAMETERIZED_H_

#include <string>

#include "ltr/parameters_container/parameters_container.h"

namespace ltr {

class Parameterized {
 public:
  virtual ~Parameterized();
  virtual void setDefaultParameters();
  virtual void checkParameters() const;
  virtual void updateParameters();

  template <class T, class TPred>
  void checkParameter(const std::string &name, TPred pred) const
      throw(std::logic_error, std::bad_cast) {
    const T &value = parameters_.Get<T>(name);
    if (!pred(value)) {
      throw std::logic_error("Error in parameter " + name + " check");
    }
  }

  template<class T>
  void setExistingParameter(const std::string &name, T value) {
    parameters_.SetExisting(name, value);
    checkParameters();
    updateParameters();
  }
  void clearParameters() {
    parameters_.Clear();
  }

  template<class T>
  void addNewParam(const std::string &name, const T &value) {
    parameters_.AddNew(name, value);
  }


  const ParametersContainer& parameters() const;

  void copyParameters(const ParametersContainer &params);


 private:
  ParametersContainer parameters_;
};
};

#endif  // LTR_INTERFACES_PARAMETERIZED_H_
