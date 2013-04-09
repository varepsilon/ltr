//  copyright 2013 Yandex

#include "ltr/utility/macros.h"
#include "ltr/optimization/functions/twice_differentiable_function.h"

namespace optimization {
void TwiceDifferentiableFunction::computeHessian(const Point& point,
                                                 Matrix* hessian) const {
  CHECK(point.size() == dimension());
  hessian->resize(dimension(), dimension());
  hessian->setZero();
  computeHessianImpl(point, hessian);
}
}
