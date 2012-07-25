// Copyright 2012 Yandex

#ifndef LTR_UTILITY_NEIGHBOR_WEIGHTER_H_
#define LTR_UTILITY_NEIGHBOR_WEIGHTER_H_

#include <string>

#include "ltr/utility/shared_ptr.h"
#include "ltr/interfaces/serializable.h"

using std::string;

namespace ltr {
namespace utility {
/**
  * Performs finding the weight of the neighbor
  * depending on the distance between objects and order of neighbor
  */
class NeighborWeighter : public Serializable {
 public:
  typedef ltr::utility::shared_ptr<NeighborWeighter> Ptr;
  virtual double getWeight(double neighbor_distance, int neighbor_order) = 0;
  virtual ~NeighborWeighter() {}
};

class InverseLinearDistance : public NeighborWeighter {
 public:
  double getWeight(double neighbor_distance, int neighbor_order) {
    return 1.0 / neighbor_distance;
  }

  string generateCppCode(const string& function_name) const {
    string result;
    result += "double " + function_name + "(double dist, double order) {\n";
    result += "  return 1.0 / dist;\n";
    result += "}\n";
    return result;
  }
 private:
  virtual string getDefaultAlias() const {return "Inverse linear distance";}
};

class InverseSquareDistance : public NeighborWeighter {
 public:
  double getWeight(double neighbor_distance, int neighbor_order) {
    return 1.0 / pow(neighbor_distance, 2.0);
  }

  string generateCppCode(const string& function_name) const {
    string result;
    result += "double " + function_name + "(double dist, double order) {\n";
    result += "  return 1.0 / pow(dist, 2.0);\n";
    result += "}\n";
    return result;
  }
 private:
  virtual string getDefaultAlias() const {return "Inverse sqaure distance";}
};

class InverseOrder : public NeighborWeighter {
 public:
  double getWeight(double neighbor_distance, int neighbor_order) {
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
