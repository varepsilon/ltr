// Copyright 2011 Yandex

#include <stdexcept>

#include "ltr/data/utility/data_set_utility.h"

namespace ltr {
namespace utility {

//template <typename TElement>
//DataSet<TElement> lightSubset(const DataSet<TElement>& data,
//                              const vector<int>& indices) {
//  DataSet<TElement> resultDataSet(data.feature_info());
//
//  for (size_t indexIdx = 0; indexIdx < indices.size(); ++indexIdx) {
//    if (indices[indexIdx] > data.size()) {
//      throw std::logic_error("Index is too large");
//    }
//    resultDataSet.elements_->push_back(data[indices[indexIdx]]);
//    resultDataSet.weights_->push_back(data.getWeight(indices[indexIdx]));
//  }
//  return resultDataSet;
//}
//
//template DataSet<Object> lightSubset<Object>(const DataSet<Object>& data,
//    const std::vector<int>& indexes);
//template DataSet<ObjectPair> lightSubset<ObjectPair>(
//    const DataSet<ObjectPair>& data,
//    const std::vector<int>& indexes);
//template DataSet<ObjectList> lightSubset<ObjectList>(
//    const DataSet<ObjectList>& data,
//    const std::vector<int>& indexes);

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
