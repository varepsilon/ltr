// Copyright 2013 Yandex

#include <math.h>
#include "ltr/utility/macros.h"
#include "ltr/optimization/test_problems/constrained_functions.h"

namespace optimization {
double G1Function::computeValueImpl(const Point& point) const {
  /**
   * \f$ f(x) = 5.0 * sum_1 - 5.0 * sum_2 - sum_3 \f$
   */
  double sum1 = 0.0;
  double sum2 = 0.0;
  double sum3 = 0.0;

  for (int coordinate = 0; coordinate < 4; ++coordinate) {
    sum1 += point(coordinate);
    sum2 += pow(point(coordinate), 2);
  }
  for (int coordinate = 4; coordinate < 13; ++coordinate) {
    sum3 += point(coordinate);
  }
  double value = 5.0 * sum1 - 5.0 * sum2 - sum3;
  return value;
}
void G1Function::computeGradientImpl(const Point& point,
                                     Vector* gradient) const {
  for (int coordinate = 0; coordinate < 4; ++coordinate) {
    (*gradient)(coordinate) = 5.0 * (1.0 - 2.0 * point(coordinate));
  }
  for (int coordinate = 4; coordinate < 13; ++coordinate) {
    (*gradient)(coordinate) = -1.0;
  }
}

void G1Function::computeHessianImpl(const Point& point, Matrix* hessian) const {
  for (int coordinate = 0; coordinate < 4; ++coordinate) {
    (*hessian)(coordinate, coordinate) = -10.0;
  }
}

double G3Function::computeValueImpl(const Point& point) const {
  double value = 1.0;
  for (int coordinate = 0; coordinate < dimension(); ++coordinate) {
    value *= point(coordinate);
  }
  value *= pow(dimension(), 1.0 / dimension());
  return value;
}

void G3Function::computeGradientImpl(const Point& point,
                                     Vector* gradient) const {
  gradient->setOnes();

  for (int first_coordinate = 0;
       first_coordinate < dimension();
       ++first_coordinate) {
    for (int second_coordinate = 0;
         second_coordinate < dimension();
         ++second_coordinate) {
      if (first_coordinate != second_coordinate) {
        (*gradient)(first_coordinate) *= point(second_coordinate);
      }
    }
  }
  (*gradient) *= pow(dimension(), 1.0 / dimension());
}

void G3Function::computeHessianImpl(const Point& point, Matrix* hessian) const {
  for (int first_coordinate = 0;
       first_coordinate < dimension();
       ++first_coordinate) {
    for (int second_coordinate = 0;
         second_coordinate < dimension();
         ++second_coordinate) {
      (*hessian)(first_coordinate, second_coordinate) =
        computePartHessian(first_coordinate, second_coordinate, point);
    }
  }
}
double G3Function::computePartHessian(int first_coordinate,
                                      int second_coordinate,
                                      const Point& point) const {
  double part_hessian = 0.0;
  /**
   * for non-diag elements
   */
  if (first_coordinate != second_coordinate) {
    part_hessian = pow(dimension(), 1.0 / dimension());
    for (int i = 1; i < dimension(); ++i) {
      if ((i != first_coordinate) && (i != second_coordinate)) {
        part_hessian *= point(i);
      }
    }
  }
  return part_hessian;
}
}
