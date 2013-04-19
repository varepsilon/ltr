// Copyright 2012 Yandex

#ifndef LTR_OPTIMIZATION_FUNCTIONS_QUADRATIC_FUNCTION_H_
#define LTR_OPTIMIZATION_FUNCTIONS_QUADRATIC_FUNCTION_H_

#include <string>
#include "ltr/utility/macros.h"
#include "ltr/optimization/functions/twice_differentiable_function.h"

namespace optimization {
/**
 * \brief Class for quadratic functions in \f$R^n\f$.
 *
 * \f$f(\overrightarrow{x}) = \overrightarrow{x}^T A \overrightarrow{x} + 
 *   B^T \overrightarrow{x} + C\f$
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

  GET_SET(Matrix, quadratic);
  GET_SET(Vector, linear);
  GET_SET(double, shift);

  string getDefaultAlias() const { return "QuadraticFunction";}
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
   * Matrix of quadratic coefficients \f$ A \f$.
   */
  Matrix quadratic_;
  /** 
   * Matrix of linear coefficients \f$ B \f$.
   */
  Vector linear_;
  /**
   * Shift term \f$ C \f$.
   */
  double shift_;
};
}


#endif  // LTR_OPTIMIZATION_FUNCTIONS_QUADRATIC_FUNCTION_H_
