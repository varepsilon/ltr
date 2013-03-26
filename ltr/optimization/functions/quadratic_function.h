// Copyright 2012 Yandex

#ifndef LTR_OPTIMIZATION_FUNCTIONS_QUADRATIC_FUNCTION_H_
#define LTR_OPTIMIZATION_FUNCTIONS_QUADRATIC_FUNCTION_H_

#include <string>
#include "ltr/utility/macros.h"
#include "ltr/optimization/functions/twice_differentiable_function.h"

namespace optimization {
/**
 * \class\brief class for quadratic functions in R^n
 * \f$f(\override(x)) = \override(x)^T A \override(x) + B^T override(x) + C$\f
 */

class QuadraticFunction : public TwiceDifferentiableFunction {
 public:
  typedef ltr::utility::shared_ptr<QuadraticFunction> Ptr;

  explicit QuadraticFunction(int dimension)
    : TwiceDifferentiableFunction(dimension) { }
  explicit QuadraticFunction(const Matrix& quadratic,
                             const Vector& linear,
                             double shift);
  ~QuadraticFunction() { }

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

  GET_SET(Matrix, quadratic);
  GET_SET(Vector, linear);
  GET_SET(double, shift);

  string getDefaultAlias() const { return "QuadraticFunction";}
 private:
  /** 
   * matrix of quadratic coefficients \f$ A $\f
   */
  Matrix quadratic_;
  /** 
   * matrix of linear coefficients \f$ B $\f
   */
  Vector linear_;
  /**
   * shift term \f$ C $\f
   */
  double shift_;
};
}


#endif  // LTR_OPTIMIZATION_FUNCTIONS_QUADRATIC_FUNCTION_H_
