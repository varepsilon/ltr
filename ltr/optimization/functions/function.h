// Copyright 2012 Yandex

#ifndef LTR_OPTIMIZATION_FUNCTIONS_FUNCTION_H_
#define LTR_OPTIMIZATION_FUNCTIONS_FUNCTION_H_

#include <vector>
#include <string>
#include "Eigen/Core"
#include "ltr/interfaces/aliaser.h"

namespace optimization {

typedef Eigen::VectorXd Point;
typedef Eigen::VectorXd Vector;
typedef Eigen::MatrixXd Matrix;

/**\class\brief abstract class for defining functions in \f$\mathbb{R}^n\f$
 */
class Function : public ltr::Aliaser {
 public:
  explicit Function(int dimension);
  virtual ~Function() { }
  /** get dimension of the function domain
  */
  int dimension() const;

  /* get value of this function in a given point
   */
  virtual double value(const Point& point) const = 0;
  virtual double operator() (const Point& point) const
    { return value(point); }
 protected:
  virtual string getDefaultAlias() const { return "Function"; }
 private:
  int dimension_;
};
}


#endif  // LTR_OPTIMIZATION_FUNCTIONS_FUNCTION_H_
