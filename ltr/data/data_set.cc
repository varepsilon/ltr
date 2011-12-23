// Copyright 2011 Yandex

#include "ltr/data/data_set.h"

namespace ltr {

template < >
template < >
DataSet<Object> DataSet<Object>::subset< std::vector<size_t> >(
    std::vector<size_t> indexes) const {
  DataSet<Object> resultDataSet(this->featureInfo());

  for (size_t elementIdx = 0; elementIdx < indexes.size(); ++elementIdx) {
    size_t elementInDataSetIdx = indexes[elementIdx];
    resultDataSet.p_Elements_->push_back((*this)[elementInDataSetIdx]);
    resultDataSet.p_Weights_->push_back((*this).getWeight(elementInDataSetIdx));
  }

  return resultDataSet;
};

template < >
template < >
DataSet<ObjectPair> DataSet<ObjectPair>::subset< std::vector<size_t> >(
    std::vector<size_t> indexes) const {
  DataSet<ObjectPair> resultDataSet(this->featureInfo());

  for (size_t elementIdx = 0; elementIdx < indexes.size(); ++elementIdx) {
    size_t elementInDataSetIdx = indexes[elementIdx];
    resultDataSet.p_Elements_->push_back((*this)[elementInDataSetIdx]);
    resultDataSet.p_Weights_->push_back((*this).getWeight(elementInDataSetIdx));
  }

  return resultDataSet;
};

template < >
template < >
DataSet<ObjectList> DataSet<ObjectList>::subset< std::vector<size_t> >(
    std::vector<size_t> indexes) const {
  DataSet<ObjectList> resultDataSet(this->featureInfo());

  for (size_t elementIdx = 0; elementIdx < indexes.size(); ++elementIdx) {
    size_t elementInDataSetIdx = indexes[elementIdx];
    resultDataSet.p_Elements_->push_back((*this)[elementInDataSetIdx]);
    resultDataSet.p_Weights_->push_back((*this).getWeight(elementInDataSetIdx));
  }

  return resultDataSet;
};
}
