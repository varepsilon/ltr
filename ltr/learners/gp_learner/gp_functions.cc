// Copyright 2011 Yandex School Practice

#include <boost/lexical_cast.hpp>

#include <string>

#include "ltr/learners/gp_learner/gp_functions.h"

using std::string;

namespace ltr {
namespace gp {

void setContextToObject(Puppy::Context* context,
                        const Object& object) {
  for (size_t feature_index = 0;
      feature_index < object.feature_count();
      ++feature_index) {
    double feature_value = object.features().at(feature_index);
    string feature_name =
      "feature[" + boost::lexical_cast<string>(feature_index);
    feature_name += "]";
    context->mPrimitiveMap[feature_name]->setValue(&feature_value);
  }
}

template <typename TElement>
void markDataSetWithTree(const DataSet<TElement>& data,
                         Puppy::Context* context,
                         Puppy::Tree* tree) {
  for (size_t element_index = 0;
      element_index < data.size(); ++element_index) {
    PerObjectAccessor<const TElement> poa(&data[element_index]);
    for (size_t object_index = 0;
        object_index < poa.object_count(); ++object_index) {
        setContextToObject(context, poa.object(object_index));
      double predicted_label;
      tree->interpret(&predicted_label, *context);
      poa.object(object_index).set_predicted_label(predicted_label);
    }
  }
}

template void markDataSetWithTree<Object>(const DataSet<Object>& data,
                                          Puppy::Context* context,
                                          Puppy::Tree* tree);
template void markDataSetWithTree<ObjectPair>(const DataSet<ObjectPair>& data,
                                              Puppy::Context* context,
                                              Puppy::Tree* tree);
template void markDataSetWithTree<ObjectList>(const DataSet<ObjectList>& data,
                                              Puppy::Context* context,
                                              Puppy::Tree* tree);

const Serializable* puppyPrimitiveHandleToPSerializable(
    const Puppy::PrimitiveHandle puppy_primitive) {
  const Serializable* serializable =
          dynamic_cast<const Serializable *>(puppy_primitive.getPointer());
  if (!serializable) {
    throw std::logic_error(
        "All primitives in GP should implement Serializable"
        "interface to generate code for GPScprer.");
  }
  return serializable;
}

void writeTreeAsStringOfCppCalls(const Puppy::Tree& tree,
                                 std::ostream &output,
                                 size_t node_index) {
  assert(node_index < tree.size());
  size_t number_arguments = tree[node_index].mPrimitive->getNumberArguments();
  if (number_arguments == 0) {
    output << tree[node_index].mPrimitive->getName();
  } else {
    const Serializable* serializable
        = puppyPrimitiveHandleToPSerializable(tree[node_index].mPrimitive);
    output << serializable->getDefaultSerializableObjectName();
    output << tree[node_index].mPrimitive->getName();
    output << "(";
    size_t current_node_index = node_index + 1;
    for (size_t argument_index = 0;
        argument_index < number_arguments; ++argument_index) {
      writeTreeAsStringOfCppCalls(tree, output, current_node_index);
      current_node_index += tree[current_node_index].mSubTreeSize;
      if ((argument_index + 1) < number_arguments) {
        output << ",";
      }
    }
    output << ")";
  }
}
}
}
