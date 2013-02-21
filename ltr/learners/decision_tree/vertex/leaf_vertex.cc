// Copyright 2012 Yandex

#include "ltr/learners/decision_tree/vertex/leaf_vertex.h"

#include "ltr/utility/boost/lexical_cast.h"

using ltr::utility::lexical_cast;

namespace ltr {
namespace decision_tree {
double LeafVertex::value(const Object& object) const {
  return value_;
}

void LeafVertex::set_value(double value) {
  value_ = value;
}

string LeafVertex::generateCppCode(const string& function_name) const {
  string hpp_code;

  hpp_code.
    append("inline double ").
    append(function_name).
    append("(const std::vector<double>& features) {return ").
    append(lexical_cast<string>(value_)).
    append(";}\n\n");

  return hpp_code;
}

string LeafVertex::generateCppCodeWithId(const string& function_name) const {
  return generateCppCode(
    function_name + "_id_" + lexical_cast<string>(this->getId()));
}

string LeafVertex::getDefaultAlias() const {
  return "Leaf Vertex";
}
};
};
