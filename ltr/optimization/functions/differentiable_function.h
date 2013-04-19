// Copyright 2012 Yandex

#ifndef LTR_OPTIMIZATION_FUNCTIONS_DIFFERENTIABLE_FUNCTION_H_
#define LTR_OPTIMIZATION_FUNCTIONS_DIFFERENTIABLE_FUNCTION_H_

#include <string>
#include "ltr/utility/boost/shared_ptr.h"
#include "ltr/optimization/functions/function.h"

namespace optimization {
/**
 * \brief Abstract class for defining differentiable functions
 * in \f$\mathbb{R}^n\f$
 *
 * \see Function
 */
class DifferentiableFunction : public Function {
 public:
  typedef ltr::utility::shared_ptr<DifferentiableFunction> Ptr;

  explicit DifferentiableFunction(int dimension)
    : Function(dimension) { }
  virtual ~DifferentiableFunction() { }

  /**
   * Compute first derivative of this function in a given point.
   */
  void computeGradient(const Point& point, Vector* gradient) const;
  virtual string getDefaultAlias() const {return "DifferentiableFunction"; }
 private:
  /**
   * Implementation of computeValue method.
   */
  virtual double computeValueImpl(const Point& point) const = 0;
  /**
   * Implementation of computeGradient method.
   */
  virtual void computeGradientImpl(const Point& point,
                                   Vector* gradient) const = 0;
};
}

#endif  // LTR_OPTIMIZATION_FUNCTIONS_DIFFERENTIABLE_FUNCTION_H_
