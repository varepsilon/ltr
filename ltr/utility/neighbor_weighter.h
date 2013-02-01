// Copyright 2012 Yandex

#ifndef LTR_UTILITY_NEIGHBOR_WEIGHTER_H_
#define LTR_UTILITY_NEIGHBOR_WEIGHTER_H_

#include <string>
#include <cmath>

#include "ltr/parameters_container/parameters_container.h"
#include "ltr/utility/boost/shared_ptr.h"
#include "ltr/interfaces/serializable.h"
#include "ltr/interfaces/parameterized.h"

using std::string;

using ltr::Parameterized;
using ltr::ParametersContainer;

namespace ltr {
namespace utility {
/**
  * Performs finding the weight of the neighbor
  * depending on the distance between objects and order of neighbor
  */
class NeighborWeighter : public Serializable, public Parameterized {
 public:
  typedef ltr::utility::shared_ptr<NeighborWeighter> Ptr;
  virtual double getWeight(double neighbor_distance, int neighbor_order) const = 0;
  virtual ~NeighborWeighter() {}
};

class InverseLinearDistance : public NeighborWeighter {
 public:

  InverseLinearDistance() {
    min_distance_ = 1e-9;
  }

  explicit InverseLinearDistance(double min_distance):
    min_distance_(min_distance) {}

  explicit InverseLinearDistance(const ParametersContainer& parameters) {
  }

  virtual double getWeight(double neighbor_distance, int neighbor_order) const {
    if (neighbor_distance < min_distance_) {
      return min_distance_;
    }
    return 1.0 / neighbor_distance;
  }

  string generateCppCode(const string& function_name) const {
    string result;
    result += "double " + function_name + "(double dist, double order) {\n";
    result += "  double min_distance = " +
      ltr::utility::lexical_cast<string, double>(min_distance_) + ";\n";
    result += "  if (dist < min_distance) {\n";
    result += "    return min_distance;\n";
    result += "  }\n";
    result += "  return 1.0 / dist;\n";
    result += "}\n";
    return result;
  }
 private:
  double min_distance_;
  virtual string getDefaultAlias() const {return "Inverse linear distance";}
};

class InversePowerDistance: public NeighborWeighter {
 public:

  InversePowerDistance() {
    power_ = 2.;
    min_distance_ = 1e-9;
  }

  explicit InversePowerDistance(double power, double min_distance = 1e-9):
    power_(power), min_distance_(min_distance) {}

  explicit InversePowerDistance(const ParametersContainer& parameters) {
  }

  virtual double getWeight(double neighbor_distance, int neighbor_order) const {
    if (neighbor_distance < min_distance_) {
      return 1. / std::pow(min_distance_, power_);
    }
    return 1.0 / std::pow(neighbor_distance, power_);
  }

  string generateCppCode(const string& function_name) const {
    string result;
    result += "double " + function_name + "(double dist, double order) {\n";
    result += "  double power = " +
      ltr::utility::lexical_cast<string, double>(power_) + ";\n";
    result += "  double min_distance = " +
      ltr::utility::lexical_cast<string, double>(min_distance_) + ";\n";
    result += "  if (dist < min_distance) {\n";
    result += "    return 1. / pow(min_distance, power);\n";
    result += "  }\n";
    result += "  return 1.0 / pow(dist, power);\n";
    result += "}\n";
    return result;
  }
 private:
  double power_;
  double min_distance_;
  virtual string getDefaultAlias() const {return "Inverse power distance";}
};

class InverseOrder : public NeighborWeighter {
 public:
  InverseOrder() {
  }

  explicit InverseOrder(const ParametersContainer& parameters) {
  }

  virtual double getWeight(double neighbor_distance, int neighbor_order) const {
    return 1.0 / (neighbor_order + 1.0);
  }

  string generateCppCode(const string& function_name) const {
    string result;
    result += "double " + function_name + "(double dist, double order) {\n";
    result += "  return 1.0 / (order + 1.0);\n";
    result += "}\n";
    return result;
  }
 private:
  virtual string getDefaultAlias() const {return "Inverse order";}
};
};
};

#endif  // LTR_UTILITY_NEIGHBOR_WEIGHTER_H_
