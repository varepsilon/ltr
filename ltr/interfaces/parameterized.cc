// Copyright 2011 Yandex

#include "ltr/interfaces/parameterized.h"

namespace ltr {

ParametersContainer& Parameterized::parameters() {
  return parameters_;
}

const ParametersContainer& Parameterized::parameters() const {
  return parameters_;
}

void Parameterized::setDefaultParameters() {
  parameters_.clear();
}

void Parameterized::checkParameters() const {}
}
