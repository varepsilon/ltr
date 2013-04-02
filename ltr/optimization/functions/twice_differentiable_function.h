// Copyright 2012 Yandex

#ifndef LTR_OPTIMIZATION_FUNCTIONS_TWICE_DIFFERENTIABLE_FUNCTION_H_
#define LTR_OPTIMIZATION_FUNCTIONS_TWICE_DIFFERENTIABLE_FUNCTION_H_

#include <string>
#include "ltr/utility/boost/shared_ptr.h"
#include "ltr/optimization/functions/differentiable_function.h"

namespace optimization {
/**
 * \class\brief abstract class for defining twice
 *  differentiable functions in \f$\mathbb{R}^n\f$
 * \see DifferentiableFunction
 */
class TwiceDifferentiableFunction : public DifferentiableFunction {
 public:
  typedef ltr::utility::shared_ptr<TwiceDifferentiableFunction> Ptr;

  explicit TwiceDifferentiableFunction(int dimension)
    : DifferentiableFunction(dimension) { }
  virtual ~TwiceDifferentiableFunction() { }

  /**
   * compute second derivative in given point
   */
  void computeHessian(const Point& point, Matrix* hessian) const;

  string getDefaultAlias() const { return "TwiceDifferentiableFunction"; }
 private:
  /**
   * implementation of computeValue method
   */
  virtual double computeValueImpl(const Point& point) const = 0;
  /**
   * implementation of computeGradient method
   */
  virtual void computeGradientImpl(const Point& point,
                                   Vector* gradient) const = 0;
  /**
   * implementation of computeHessian method
   */
  virtual void computeHessianImpl(const Point& point,
                                  Matrix* hessian) const = 0;
};
}

#endif  // LTR_OPTIMIZATION_FUNCTIONS_TWICE_DIFFERENTIABLE_FUNCTION_H_
