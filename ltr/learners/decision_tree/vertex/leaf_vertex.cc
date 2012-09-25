// Copyright 2012 Yandex

#include "boost/lexical_cast.hpp"

#include "ltr/learners/decision_tree/vertex/leaf_vertex.h"

using boost::lexical_cast;

namespace ltr {
namespace decision_tree {
double LeafVertex::value(const Object& object) const {
  return value_;
}

void LeafVertex::set_value(double value) {
  value_ = value;
}

string LeafVertex::generateVertexCppCode(
  const string& function_name, int level_id, int vertex_id) const {
    string hpp_code;

    hpp_code.
      append("inline double ").
      append(function_name).
      append("_level_" + lexical_cast<string>(level_id)).
      append("_vertex_" + lexical_cast<string>(vertex_id)).
      append("(const std::vector<double>& features) {return ").
      append(lexical_cast<string>(value_)).
      append(";}\n");

    return hpp_code;
}

string LeafVertex::getDefaultAlias() const {
  return "Leaf Vertex";
}
};
};
