// Copyright 2013 Yandex

#ifndef LTR_OPTIMIZATION_FUNCTIONS_LINEAR_FUNCTION_H_
#define LTR_OPTIMIZATION_FUNCTIONS_LINEAR_FUNCTION_H_

#include <string>
#include "ltr/utility/macros.h"
#include "ltr/optimization/functions/twice_differentiable_function.h"

namespace optimization {
/**
 * \brief Class for defining linear functions in \f$\mathbb{R}^n\f$.
 *
 * \f$f(x_1...x_n) = a_1*x_1 + ..+ a_n*x_n + b\f$
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

  GET_SET(Vector, factor);
  GET_SET(double, shift);

  string getDefaultAlias() const { return "LinearFunction";}
 private:
  /**
   * Implementation of computeValue method.
   */
  double computeValueImpl(const Point& point) const;
  /**
   * Implementation of computeGradient method.
   */
  void computeGradientImpl(const Point& point, Vector* gradient) const;
  /**
   * Implementation of computeHessian method.
   */
  void computeHessianImpl(const Point& point, Matrix* hessian) const;
  /**
   * Vector of coefficients in linear form.
   */
  Vector factor_;
  /**
   * Free term in linear form.
   */
  double shift_;
};
}

#endif  // LTR_OPTIMIZATION_FUNCTIONS_LINEAR_FUNCTION_H_
