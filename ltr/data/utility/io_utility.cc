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

void groupByMeta(const vector<Object>& objects,
                 const string& group_parameter,
                 map<string, vector<Object> >* result) {
  result->clear();
  for (int object_index = 0; object_index < objects.size(); ++object_index) {
    string value = objects[object_index].getMetaInfo(group_parameter);
    (*result)[value].push_back(objects[object_index]);
  }
}

void groupByIntMeta(const vector<Object>& objects,
                    const string& group_parameter,
                    map<int, vector<Object> >* result) {
  result->clear();
  for (int object_index = 0; object_index < objects.size(); ++object_index) {
    try {
      string value = objects[object_index].getMetaInfo(group_parameter);
      (*result)[boost::lexical_cast<int>(value)].push_back(objects[object_index]);
    } catch(boost::bad_lexical_cast) {
      throw std::logic_error("can't group objects by " + group_parameter);
    }
  }
}

void groupByFloatMeta(const vector<Object>& objects,
                      const string& group_parameter,
                      map<float, vector<Object> >* result) {
  result->clear();
  for (int object_index = 0; object_index < objects.size(); ++object_index)
    try {
      string value = objects[object_index].getMetaInfo(group_parameter);
      (*result)[boost::lexical_cast<float>(value)].push_back(objects[object_index]);
    } catch(boost::bad_lexical_cast) {
      throw std::logic_error("can't group objects by " + group_parameter);
    }
}
}
}
