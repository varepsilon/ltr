// Copyright 2011 Yandex

#include "ltr/data/utility/data_set_converters.h"

namespace ltr {
namespace utility {

void ListToAllPairsConverter::operator()(const ObjectList& list,
                                         vector<ObjectPair>* result) {
  result->clear();
  for (int first_index = 0; first_index < (int)list.size(); ++first_index) {
    for (int second_index = 0; second_index < first_index; ++second_index) {
      result->push_back(ObjectPair(list[first_index], list[second_index]));
    }
  }
}

template<>
DataSet<ObjectPair> convertDataSet(const DataSet<ObjectList>& data,
                                   ListToPairConvertionStrategy::Ptr strategy) {
    DataSet<ObjectPair> result;
    for (int list_index = 0; list_index < (int)data.size(); ++list_index) {
      vector<ObjectPair> pairs;
      (*strategy)(data[list_index], &pairs);
      for (int pair_index = 0; pair_index < (int)pairs.size(); ++pair_index) {
        result << pairs[pair_index];
      }
    }
    return result;
}

template<>
DataSet<Object> convertDataSet(const DataSet<ObjectList>& data,
                               ListToPairConvertionStrategy::Ptr strategy) {
  DataSet<Object> result;
  for (int list_index = 0; list_index < (int)data.size(); ++list_index) {
    for (int object_index = 0;
         object_index < data[list_index].size();
         ++object_index) {
      result << data[list_index][object_index];
    }
  }
  return result;
}

template<>
DataSet<Object> convertDataSet(const DataSet<ObjectPair>& data,
                               ListToPairConvertionStrategy::Ptr strategy) {
  DataSet<Object> result;
  for (int pair_index = 0; pair_index < (int)data.size(); ++pair_index) {
    PerObjectAccessor<const ObjectPair> poa(&data[pair_index]);
    for (int object_index = 0;
         object_index < poa.object_count();
         ++object_index) {
      result << poa.object(object_index);
    }
  }
  return result;
}
}
}
