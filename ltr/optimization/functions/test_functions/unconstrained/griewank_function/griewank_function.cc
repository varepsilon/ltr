// Copyright 2012 Yandex

#include "ltr/optimization/functions/test_functions/unconstrained/griewank_function/griewank_function.h"
#include <math.h>
#include "ltr/utility/macros.h"

namespace optimization {
double GriewankFunction::value(const Point& point) const {
  CHECK(point.size() == dimension());

  double sum = 0.0;
  double production = 1.0;

  for (int i = 0; i < dimension(); ++i) {
    sum += pow(point(i), 2);
    production *= cos(point(i) / pow(i + 1, 0.5));
  }

  double value = sum / 4000.0 - production + 1.0;
  return value;
}

Vector GriewankFunction::gradient(const Point& point) const {
  CHECK(point.size() == dimension());

  Vector gradient(dimension());
  double production = 1.0;

  for (int i = 0; i < dimension(); ++i) {
    gradient(i) = point(i) / 2000.0;
    production = 1.0;

    for (int j = 0; j < dimension(); ++j) {
      if (j != i) {
        production *= cos(point(j) / pow(j + 1, 0.5));
      }
    }

    production *= sin(point(i) / pow(i + 1, 0.5)) / pow(i + 1, 0.5);
    gradient(i) += production;
  }

  return gradient;
}

Matrix GriewankFunction::hessian(const Point& point) const {
  CHECK(point.size() == dimension());

  Matrix hessian(dimension(), dimension());
  double production = 1.0;

  for (int i = 0; i < dimension(); ++i) {
    hessian(i, i) = 1.0 / 2000.0;
    production = 1.0;

    for (int j = 0; j < dimension(); ++j)  {
      production *= cos(point(j) / pow(j + 1, 0.5));
    }

    hessian(i, i) += production / (i + 1);
  }

  for (int i = 0; i < dimension(); ++i) {
    for (int j = 0; j < dimension(); ++j) {
      if (j != i) {
        double production = 1.0;

        for (int k = 0; k < dimension(); ++k) {
          if ((k != i) && (k != j)) {
            production *= cos(point(k) / pow(k + 1, 0.5));
          }
        }

        hessian(i, j) = production * sin(point(i) / pow(i + 1, 0.5)) *
          sin(point(j) / pow(j + 1, 0.5)) /
          (pow(j + 1, 0.5) * pow(i + 1, 0.5));
      }
    }
  }
  return hessian;
}
}
