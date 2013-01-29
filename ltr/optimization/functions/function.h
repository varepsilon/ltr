// Copyright 2012 Yandex

#ifndef LTR_OPTIMIZATION_FUNCTIONS_FUNCTION_H_
#define LTR_OPTIMIZATION_FUNCTIONS_FUNCTION_H_

#include <vector>
#include "Eigen/Core"

namespace optimization {

typedef Eigen::VectorXd Point;
typedef Eigen::VectorXd Vector;
typedef Eigen::MatrixXd Matrix;

/**\class\brief abstract class for defining functions in \f$\mathbb{R}^n\f$
 */
class Function {
 public:
  explicit Function(int dimension);
  virtual ~Function();

  int dimension() const;

  /* get value of this function in a given point
   */
  virtual double value(const Point& point) const = 0;
 private:
  int dimension_;
};
}


#endif  // LTR_OPTIMIZATION_FUNCTIONS_FUNCTION_H_
