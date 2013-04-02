// Copyright 2012 Yandex

#ifndef LTR_OPTIMIZATION_FUNCTIONS_DIFFERENTIABLE_FUNCTION_H_
#define LTR_OPTIMIZATION_FUNCTIONS_DIFFERENTIABLE_FUNCTION_H_

#include <string>
#include "ltr/utility/boost/shared_ptr.h"
#include "ltr/optimization/functions/function.h"

namespace optimization {
/**
 * \class\brief abstract class for defining
 *  differentiable functions in \f$\mathbb{R}^n\f$
 * \see Function
 */
class DifferentiableFunction : public Function {
 public:
  typedef ltr::utility::shared_ptr<DifferentiableFunction> Ptr;

  explicit DifferentiableFunction(int dimension)
    : Function(dimension) { }
  virtual ~DifferentiableFunction() { }

  /**
   * compute value of this function in a given point
   */
  virtual double computeValue(const Point& point) const = 0;
  /**
   * compute first derivative of this function in a given point
   */
  virtual void computeGradient(const Point& point, Vector* gradient) const = 0;

  virtual string getDefaultAlias() const { return "DifferentiableFunction";}
};
}

#endif  // LTR_OPTIMIZATION_FUNCTIONS_DIFFERENTIABLE_FUNCTION_H_
