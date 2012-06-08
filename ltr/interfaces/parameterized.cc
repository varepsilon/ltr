// Copyright 2011 Yandex

#include "ltr/interfaces/parameterized.h"

namespace ltr {

Parameterized::~Parameterized() { }

/**
* By default simply clears all parameters
*/
void Parameterized::setDefaultParameters() {
  parameters_.Clear();
}

void Parameterized::checkParameters() const {
}

/**
* This function is called after updation of any parameter.
* By default does nothing.
*/
void Parameterized::parametersUpdateCallback() {
}

/**
 * Returns parametersContainer of this parametrized object.
 */
const ParametersContainer& Parameterized::parameters() const {
  return parameters_;
}

void Parameterized::copyParameters(const ParametersContainer &parameters) {
  parameters_.Copy(parameters);
  checkParameters();
  parametersUpdateCallback();
}

void Parameterized::setParameters(const ParametersContainer &parameters) {
  setParametersImpl(parameters);
  checkParameters();
  parametersUpdateCallback();
}

void Parameterized::setParametersImpl(const ParametersContainer &parameters) {
}
}
