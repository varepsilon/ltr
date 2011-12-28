// Copyright 2011 Yandex

#include "ltr/data/utility/data_set_converters.h"

namespace ltr {
namespace utility {

void DefaultConverter::operator()(const ObjectList& list,
                                vector<ObjectPair>* result) {
  result->clear();
  for (int i = 0; i < list.size(); ++i) {
    for (int j = 0; j < i; ++j) {
      result->push_back(ObjectPair(list[i], list[j]));
    }
  }
}

template<>
DataSet< ObjectPair > convertDataSet(const DataSet<ObjectList>& data,
    ListToPairConvertionStrategy::Ptr strategy) {
    DataSet< ObjectPair > result;
    for (int i = 0; i < data.size(); i++) {
      vector< ObjectPair > tmp;
      (*strategy)(data[i], &tmp);
      for (int j = 0; j < tmp.size(); j++)
        result << tmp[j];
    }
    return result;
}

template<>
DataSet< Object > convertDataSet(const DataSet<ObjectList>& data,
    ListToPairConvertionStrategy::Ptr strategy) {
  DataSet< Object > result;
  for (int i = 0; i < data.size(); i++) {
    for (int j = 0; j < data[i].size(); j++) {
      result << data[i][j];
    }
  }
  return result;
}

template<>
DataSet< Object > convertDataSet(const DataSet<ObjectPair>& data,
    ListToPairConvertionStrategy::Ptr strategy) {
  DataSet< Object > result;
  for (int i = 0; i < data.size(); i++) {
    for (int j = 0; j < data[i].size(); j++) {
      result << data[i][j];
    }
  }
  return result;
}
}
}
