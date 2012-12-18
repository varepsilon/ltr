// Copyright 2012 Yandex

#ifndef LTR_OPTIMIZATION_FUNCTIONS_FUNCTION_H_
#define LTR_OPTIMIZATION_FUNCTIONS_FUNCTION_H_

#include <vector>
#include "Eigen/Core"

namespace optimization {

typedef Eigen::VectorXd Point;
typedef Eigen::VectorXd Vector;
typedef Eigen::MatrixXd Matrix;

class Function {
 public:
  explicit Function(int dimension);
  virtual ~Function();

  int dimension() const;
  virtual double value(const Point& point) const = 0;
 private:
  int dimension_;
};
}


#endif  // LTR_OPTIMIZATION_FUNCTIONS_FUNCTION_H_