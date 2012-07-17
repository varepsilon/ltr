// Copyright 2011 Yandex

#include "ltr/interfaces/parameterized.h"

namespace ltr {

Parameterized::~Parameterized() { }

/**
* By default simply clears all parameters
*/
void Parameterized::setDefaultParameters() {
}

void Parameterized::checkParameters() const {
}

void Parameterized::setParameters(const ParametersContainer& parameters) {
  setParametersImpl(parameters);
  checkParameters();
}

void Parameterized::setParametersImpl(const ParametersContainer &parameters) {
}
}
