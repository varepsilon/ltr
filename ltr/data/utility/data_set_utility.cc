// Copyright 2011 Yandex

#include <stdexcept>

#include "ltr/data/utility/data_set_utility.h"

namespace ltr {
namespace utility {

template <typename TElement>
DataSet<TElement> lightSubset(const DataSet<TElement>& data,
    const std::vector<size_t>& indexes) {
  DataSet<TElement> resultDataSet(data.featureInfo());

  for (size_t indexIdx = 0; indexIdx < indexes.size(); ++indexIdx) {
    resultDataSet.p_Elements_->push_back(data[indexes[indexIdx]]);
    resultDataSet.p_Weights_->push_back(data.getWeight(indexes[indexIdx]));
  }
  return resultDataSet;
}

template DataSet<Object> lightSubset<Object>(const DataSet<Object>& data,
    const std::vector<size_t>& indexes);
template DataSet<ObjectPair> lightSubset<ObjectPair>(
    const DataSet<ObjectPair>& data,
    const std::vector<size_t>& indexes);
template DataSet<ObjectList> lightSubset<ObjectList>(
    const DataSet<ObjectList>& data,
    const std::vector<size_t>& indexes);

void groupByMeta(const vector<Object>& objects,
    string group_parameter,
    map<string, vector<Object> >* result) {
  result->clear();
  for (size_t i = 0; i < objects.size(); i++)
    (*result)[objects[i].getMetaInfo(group_parameter)].push_back(objects[i]);
}

void groupByIntMeta(const vector<Object>& objects,
    string group_parameter,
    map<int, vector<Object> >* result) {
  result->clear();
  for (size_t i = 0; i < objects.size(); i++)
    try {
      int tm = lexical_cast<int>(objects[i].getMetaInfo(group_parameter));
      (*result)[tm].push_back(objects[i]);
    } catch(boost::bad_lexical_cast) {
      throw std::logic_error("can't group objects by " + group_parameter);
    }
}

void groupByFloatMeta(const vector<Object>& objects,
    string group_parameter,
    map<float, vector<Object> >* result) {
  result->clear();
  for (size_t i = 0; i < objects.size(); i++)
    try {
      int tm = lexical_cast<float>(objects[i].getMetaInfo(group_parameter));
      (*result)[tm].push_back(objects[i]);
    } catch(boost::bad_lexical_cast) {
      throw std::logic_error("can't group objects by " + group_parameter);
    }
}
}
}
