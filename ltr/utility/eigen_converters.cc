// Copyright 2012 Yandex

#include "ltr/utility/eigen_converters.h"

namespace ltr {
namespace utility {
VectorXd StdVectorToEigenVector(const vector<double>& std_vector) {
  VectorXd result(std_vector.size());
  for (int element_index = 0;
       element_index < std_vector.size();
       ++element_index) {
    result(element_index) = std_vector[element_index];
  }
  return result;
}
};
};
