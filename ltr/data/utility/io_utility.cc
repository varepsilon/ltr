// Copyright 2011 Yandex

#include "ltr/data/utility/io_utility.h"

namespace ltr {
namespace io_utility {

template<>
DataSet<Object> buildDataSet(Parser::Ptr parser,
    const vector<Object>& objects,
    const FeatureInfo& info) {
  DataSet<Object> data(info);
  typedef map<size_t, vector<Object> >::const_iterator object_iterator;

  for (size_t i = 0; i < objects.size(); i++)
    data << objects[i];
  return data;
}

template<>
DataSet<ObjectPair> buildDataSet(Parser::Ptr parser,
    const vector<Object>& objects,
    const FeatureInfo& info) {
  return parser->buildPairwiseDataSet(objects, info);
}

template<>
DataSet<ObjectList> buildDataSet(Parser::Ptr parser,
    const vector<Object>& objects,
    const FeatureInfo& info) {
  return parser->buildListwiseDataSet(objects, info);
}
}
}
