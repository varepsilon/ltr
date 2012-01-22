// Copyright 2011 Yandex School Practice

#include <boost/lexical_cast.hpp>

#include <string>

#include "ltr/learners/utility/gp_functions.h"

using std::string;

namespace ltr {
namespace gp {


void setContextToObject(Puppy::Context* context, const Object& obj) {
  for (size_t featureIdx = 0;
      featureIdx < obj.featureCount();
      ++featureIdx) {
    double featureVal = obj.features().at(featureIdx);
    string featureName = "F" + boost::lexical_cast<string>(featureIdx);
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
}
}
