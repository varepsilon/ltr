// Copyright 2013 Yandex

#ifndef LTR_OPTIMIZATION_FUNCTIONS_LINEAR_FUNCTION_H_
#define LTR_OPTIMIZATION_FUNCTIONS_LINEAR_FUNCTION_H_

#include <string>
#include "ltr/utility/macros.h"
#include "ltr/optimization/functions/twice_differentiable_function.h"

namespace optimization {
/**
 * \class\brief class for defining
 *  linear functions in \f$\mathbb{R}^n\f$
 *
 *  \f$f(x_1...x_n) = a_1*x_1 + ..+ a_n*x_n + b\f$
 * \see Function
 */

class LinearFunction : public TwiceDifferentiableFunction {
 public:
  typedef ltr::utility::shared_ptr<LinearFunction> Ptr;

  explicit LinearFunction(int dimension)
    : TwiceDifferentiableFunction(dimension) { }
  explicit LinearFunction(Vector factor , double shift = 0.0)
    : factor_(factor),
      shift_(shift),
      TwiceDifferentiableFunction(factor.size()) { }
  ~LinearFunction() { }

  /**
   * compute value of this function in a given point
   */
  double computeValue(const Point& point) const;
  /**
   * compute first derivative of this function in a given point
   */
  void computeGradient(const Point& point, Vector* gradient) const;
  /**
   * compute second derivative in given point
   */
  void computeHessian(const Point& point, Matrix* hessian) const;

  GET_SET(Vector, factor);
  GET_SET(double, shift);

  string getDefaultAlias() const { return "LinearFunction";}
 private:
  /**
   * vector of coefficients in linear form
   */
  Vector factor_;
  /**
   * free term in linear form
   */
  double shift_;
};
}

#endif  // LTR_OPTIMIZATION_FUNCTIONS_LINEAR_FUNCTION_H_
