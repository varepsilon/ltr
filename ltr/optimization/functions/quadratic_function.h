// Copyright 2012 Yandex

#ifndef LTR_OPTIMIZATION_FUNCTIONS_QUADRATIC_FUNCTION_H_
#define LTR_OPTIMIZATION_FUNCTIONS_QUADRATIC_FUNCTION_H_

#include <string>
#include "ltr/optimization/functions/twice_differentiable_function.h"
#include "ltr/utility/macros.h"

namespace optimization {
/**\class\brief class for quadratic functions in R^n
 * \f$f(\override(x)) = \override(x)^T A \override(x) + B^T override(x) + C$\f
 */

class QuadraticFunction : public TwiceDifferentiableFunction {
 public:
  explicit QuadraticFunction(int dimension)
    : TwiceDifferentiableFunction(dimension) { }
  explicit QuadraticFunction(Matrix a, Vector b, double c);
  ~QuadraticFunction() { }

  double value(const Point& point) const;
  Matrix hessian(const Point& point) const;
  Vector gradient(const Point& point) const;

  GET_SET(Matrix, a);
  GET_SET(Vector, b);
  GET_SET(double, c);

  string getDefaultAlias() const { return "QuadraticFunction";}
 private:
  /** matrix of quadratic coefficients \f$ A $\f
  */
  Matrix a_;
  /** matrix of linear coefficients \f$ B $\f
  */
  Vector b_;
  /** absolute term \f$ C $\f
  */
  double c_;
};
}


#endif  // LTR_OPTIMIZATION_FUNCTIONS_QUADRATIC_FUNCTION_H_
