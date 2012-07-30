// Copyright 2012 Yandex

#include <logog/logog.h>

#include "ltr/scorers/gp_scorer.h"

namespace ltr {
namespace gp {
template <typename TElement>
void GPScorer::markDataSet(const DataSet<TElement>& data) const {
  for (int element_index = 0;
       element_index < (int)data.size(); ++element_index) {
    PerObjectAccessor<const TElement> per_object_accessor(&data[element_index]);
    for (int object_index = 0;
         object_index < per_object_accessor.object_count(); ++object_index) {
      setContextToObject(per_object_accessor.object(object_index));
      double predicted_label;
      best_tree_.interpret(&predicted_label, context_);
      per_object_accessor.object(object_index).set_predicted_label(
        predicted_label);
    }
  }
}

template void GPScorer::
  markDataSet<Object>(const DataSet<Object>& data) const;
template void GPScorer::
  markDataSet<ObjectPair>(const DataSet<ObjectPair>& data) const;
template void GPScorer::
  markDataSet<ObjectList>(const DataSet<ObjectList>& data) const;

const Serializable* GPScorer::puppyPrimitiveHandleToPSerializable(
    const Puppy::PrimitiveHandle puppy_primitive) const {
  const Serializable* serializable =
    dynamic_cast<const Serializable*>(puppy_primitive.getPointer());
  if (!serializable) {
    throw std::logic_error(
      "All primitives in GP should implement Serializable"
      "interface to generate code for GPScprer.");
  }
  return serializable;
}

double GPScorer::scoreImpl(const Object& object) const {
  INFO("Starting to score an object with the gp_learner");
  assert(feature_count_ == object.feature_count());
  setContextToObject(object);
  double score;
  best_tree_.interpret(&score, context_);
  return score;
}

string GPScorer::generateCppCodeImpl(const string& function_name) const {
  string code;
  code.append("#include <vector>\n");
  // generate primitive-functions code.
  vector<Puppy::PrimitiveHandle>::const_iterator function_iterator =
    context_.mFunctionSet.begin();
  for (; function_iterator != context_.mFunctionSet.end();
       ++function_iterator) {
    const Serializable* serializable = puppyPrimitiveHandleToPSerializable(
      *function_iterator);
    string primirive_function_name =
      serializable->getDefaultSerializableObjectName();
    primirive_function_name += (*function_iterator)->getName();
    code.append(serializable->generateCppCode(primirive_function_name));
  }
  // generate the function from tree.
  stringstream sstream_for_calls;
  writeTreeAsStringOfCppCalls(sstream_for_calls, 0);
  // generate scoring function
  code.append("double ");
  code.append(function_name);
  code.append("(const std::vector< double >& feature) {\n");
  code.append("  return ");
  code.append(sstream_for_calls.str());
  code.append(";\n");
  code.append("}\n");

  return code;
}

void GPScorer::setContextToObject(const Object& object) const {
  for (int feature_index = 0;
       feature_index < object.feature_count();
       ++feature_index) {
    double feature_value = object.features().at(feature_index);
    string feature_name =
      "feature[" + boost::lexical_cast<string>(feature_index);
    feature_name += "]";
    context_.mPrimitiveMap[feature_name]->setValue(&feature_value);
  }
}

void GPScorer::writeTreeAsStringOfCppCalls(std::ostream &output, // NOLINT
                                           int node_index) const {
  assert(node_index < best_tree_.size());
  int number_arguments =
    best_tree_[node_index].mPrimitive->getNumberArguments();
  if (number_arguments == 0) {
    output << best_tree_[node_index].mPrimitive->getName();
  } else {
    const Serializable* serializable
      = puppyPrimitiveHandleToPSerializable(best_tree_[node_index].mPrimitive);
    output << serializable->getDefaultSerializableObjectName();
    output << best_tree_[node_index].mPrimitive->getName();
    output << "(";
    int current_node_index = node_index + 1;
    for (int argument_index = 0;
         argument_index < number_arguments; ++argument_index) {
      writeTreeAsStringOfCppCalls(output, current_node_index);
      current_node_index += best_tree_[current_node_index].mSubTreeSize;
      if ((argument_index + 1) < number_arguments) {
        output << ",";
      }
    }
    output << ")";
  }
}
};
};
