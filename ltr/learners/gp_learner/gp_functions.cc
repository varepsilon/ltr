// Copyright 2011 Yandex School Practice

#include <boost/lexical_cast.hpp>

#include <string>

#include "ltr/learners/gp_learner/gp_functions.h"

using std::string;

namespace ltr {
namespace gp {

void setContextToObject(Puppy::Context* context, const Object& obj) {
  for (size_t featureIdx = 0;
      featureIdx < obj.featureCount();
      ++featureIdx) {
    double featureVal = obj.features().at(featureIdx);
    string featureName = "feature[" + boost::lexical_cast<string>(featureIdx);
    featureName += "]";
    context->mPrimitiveMap[featureName]->setValue(&featureVal);
  }
}

template <typename TElement>
void markDataSetWithTree(const DataSet<TElement>& data,
    Puppy::Context* context, Puppy::Tree* tree) {
  for (size_t elementIdx = 0; elementIdx < data.size(); ++elementIdx) {
    for (size_t objectIdx = 0; objectIdx < data[elementIdx].size();
        ++objectIdx) {
      setContextToObject(context, data[elementIdx][objectIdx]);
      double resultFromTree;
      tree->interpret(&resultFromTree, *context);
      data[elementIdx][objectIdx].setPredictedLabel(resultFromTree);
    }
  }
}

template void markDataSetWithTree<Object>(const DataSet<Object>& data,
    Puppy::Context* context, Puppy::Tree* tree);
template void markDataSetWithTree<ObjectPair>(const DataSet<ObjectPair>& data,
    Puppy::Context* context, Puppy::Tree* tree);
template void markDataSetWithTree<ObjectList>(const DataSet<ObjectList>& data,
    Puppy::Context* context, Puppy::Tree* tree);

const Serializable* puppyPrimitiveHandleToPSerializable(
    const Puppy::PrimitiveHandle mPrimitive) {
  const Serializable* pSerializable =
          dynamic_cast<const Serializable *>(&(*(mPrimitive)));
  if (!pSerializable) {
    throw std::logic_error(
        "All primitives in GP should implement Serializable"
        "interface to generate code for GPScprer.");
  }
  return pSerializable;
}

void writeTreeAsStringOfCppCalls(
    const Puppy::Tree& tree,
    std::ostream* pIoOS,
    size_t inIndex) {
  assert(inIndex < tree.size());
  std::ostream& ioOS = *pIoOS;
  size_t nbArgs = tree[inIndex].mPrimitive->getNumberArguments();
  if (nbArgs == 0) {
    ioOS << tree[inIndex].mPrimitive->getName();
  } else {
    const Serializable* pSerializable = puppyPrimitiveHandleToPSerializable(
        tree[inIndex].mPrimitive);
    ioOS << pSerializable->getDefaultSerializableObjectName();
    ioOS << tree[inIndex].mPrimitive->getName();
    ioOS << "(";
    size_t idxToCallWith = inIndex + 1;
    for (size_t argIdx = 0; argIdx < nbArgs; ++argIdx) {
      writeTreeAsStringOfCppCalls(tree, &ioOS, idxToCallWith);
      idxToCallWith += tree[idxToCallWith].mSubTreeSize;
      if ((argIdx + 1) < nbArgs) {
        ioOS << ",";
      }
    }
    ioOS << ")";
  }
}
}
}
