//  copyright 2013 Yandex

#include "ltr/utility/macros.h"
#include "ltr/optimization/functions/differentiable_function.h"

namespace optimization {
void DifferentiableFunction::computeGradient(const Point& point,
                                             Vector* gradient) const {
  CHECK(point.size() == dimension());
  gradient->resize(dimension());
  gradient->setZero();
  computeGradientImpl(point, gradient);
}
}
