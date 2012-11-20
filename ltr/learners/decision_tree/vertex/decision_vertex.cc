// Copyright 2012 Yandex

#include <string>

#include "boost/lexical_cast.hpp"

#include "ltr/learners/decision_tree/vertex/decision_vertex.h"

#include "ltr/utility/numerical.h"

using ltr::utility::lexical_cast;

using ltr::utility::Inf;

namespace ltr {
namespace decision_tree {
double DecisionVertex::value(const Object& object) const {
  if (!this->hasChild()) {
    throw std::logic_error("Non leaf vertex has no children");
  }

  Vertex::Ptr best_child = findBestChild(object);
  return best_child->value(object);
}

string DecisionVertex::generateCppCode(const string& function_name) const {
  return generateCppCodeImpl(function_name, false);
}

string DecisionVertex::generateCppCodeWithId(
  const string& function_name) const {
    return generateCppCodeImpl(function_name, true);
}

Vertex::Ptr DecisionVertex::findBestChild(const Object& object) const {
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
  return best_child;
}

string DecisionVertex::generateCppCodeImpl(
  const string& function_name, bool add_id) const {
    string hpp_code;

    for (int child_index = 0;
         child_index < (int)childrens_.size(); ++child_index) {
      hpp_code += childrens_[child_index]->generateCppCodeWithId(function_name);
      hpp_code +=
        conditions_[child_index]->generateCppCode(function_name + "_condition");
    }

    hpp_code.
      append("inline double ").
      append(function_name);
    if (add_id) {
      hpp_code.append("_id_" + lexical_cast<string>(this->getId()));
    }
    hpp_code.
      append("(const std::vector<double>& features) {\n").
      append("  double best_val = -1e9;\n").
      append("  double res;\n");
    for (int child_index = 0;
         child_index < (int)childrens_.size(); ++child_index) {
      hpp_code.
        append("  double val" + lexical_cast<string>(child_index)).
        append(" = " + function_name + "_condition_id_").
        append(lexical_cast<string>(conditions_[child_index]->getId())).
        append("(features);\n").
        append("  if (val" + lexical_cast<string>(child_index)).
        append(" > best_val) {\n    best_val = val").
        append(lexical_cast<string>(child_index) + ";\n").
        append("    res = " + function_name + "_id_").
        append(lexical_cast<string>(childrens_[child_index]->getId())).
        append("(features);\n  }\n");
    }
    hpp_code.
      append("  return res;\n").
      append("}\n\n");

    return hpp_code;
}

string DecisionVertex::getDefaultAlias() const {
  return "Decision Vertex";
}
};
};
