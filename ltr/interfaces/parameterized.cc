// Copyright 2011 Yandex

#include <string>

#include "ltr/interfaces/parameterized.h"

using std::string;

namespace ltr {
  ParametersContainer& Parameterized::parameters() {
    return parameters_;
  }

  const ParametersContainer& Parameterized::parameters() const {
    return parameters_;
  }

  void Parameterized::setDefaultParameters() {
  }

  string Parameterized::checkParameters() const {
    return "";
  }
};
