// Copyright 2012 Yandex

#include <string>

#include "boost/lexical_cast.hpp"

#include "ltr/learners/decision_tree/vertex/decision_vertex.h"

#include "ltr/utility/numerical.h"

using boost::lexical_cast;

using ltr::utility::Inf;

namespace ltr {
namespace decision_tree {
double DecisionVertex::value(const Object& object) const {
  if (!this->hasChild()) {
    throw std::logic_error("Non leaf vertex has no children");
  }

  Vertex::Ptr best_child;
  double best_value = -Inf;

  for (int child_index = 0; child_index < (int)childrens_.size();
       ++child_index) {
    double current_value = conditions_[child_index]->value(object);
    if (current_value > best_value) {
      best_value = current_value;
      best_child = childrens_[child_index];
    }
  }

  return best_child->value(object);
}

string DecisionVertex::generateVertexCppCode(
  const string& function_name, int level_id, int vertex_id) const {
    string hpp_code;

    for (int child_index = 0;
         child_index < (int)childrens_.size(); ++child_index) {
      hpp_code += childrens_[child_index]->
        generateVertexCppCode(function_name, level_id + 1, child_index);
    }

    hpp_code.
      append("inline double ").
      append(function_name).
      append("_level_" + lexical_cast<string>(level_id)).
      append("_vertex_" + lexical_cast<string>(vertex_id)).
      append("(const std::vector<double>& features) {\n").
      append("  double best_value = -1e9;\n");
    for (int child_index = 0;
         child_index < (int)childrens_.size(); ++child_index) {
      hpp_code.
        append("  best_value = std::max(best_value, " + function_name).
        append("_level_" + lexical_cast<string>(level_id + 1)).
        append("_vertex_" + lexical_cast<string>(child_index)).
        append("(features));\n");
    }
    hpp_code.
      append("  return best_value;\n").
      append("}\n");

    return hpp_code;
}

string DecisionVertex::getDefaultAlias() const {
  return "Decision Vertex";
}
};
};
