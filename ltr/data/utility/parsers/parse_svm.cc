
// Copyright 2013 Yandex

#include <sstream>
#include <stdexcept>
#include <utility>

#include "ltr/data/utility/parsers/parse_svm.h"
#include "ltr/data/utility/io_utility.h"
#include "ltr/utility/numerical.h"
#include "ltr/utility/boost/string_utils.h"
#include "ltr/utility/boost/lexical_cast.hpp"

using ltr::utility::isNaN;
using ltr::utility::lexical_cast;
using ltr::utility::NaN;
using ltr::utility::split;
using ltr::utility::trim;
using ltr::utility::trim_copy;

using std::endl;
using std::logic_error;
using std::make_pair;
using std::map;
using std::pair;
using std::streampos;
using std::string;
using std::stringstream;
using std::vector;

namespace ltr {
namespace io_utility {
void parseSVMLine(const string& line, double* relevance, double* qid,
                  vector<pair<int, double> > *feature_values) {
  *qid = NaN;
  feature_values->clear();
  vector<string> parse_data;
  split(trim_copy(line), "#", &parse_data);
  if (parse_data.size() == 0) {
    throw logic_error("no feature_values");
  }
  trim(&parse_data);
  if (parse_data[0].size() == 0) {
    throw Parser::bad_line();
  }
  string features_line = parse_data[0];
  split(features_line, &parse_data);

  *relevance = lexical_cast<double>(parse_data.front());

  for (int value_index = 1; value_index < parse_data.size(); ++value_index) {
    vector<string> feature;
    split(parse_data[value_index], ":", &feature);
    trim(&parse_data);
    if (feature[0] == "qid") {
      *qid = lexical_cast<int>(feature[1]);
    } else if (feature.size() == 2) {
      feature_values->push_back(make_pair(lexical_cast<int>(feature[0]),
                                          lexical_cast<double>(feature[1])));
    } else {
      throw logic_error("can't parse feature " + parse_data[value_index]);
    }
  }
}

void SVMParser::parseDataInfo(istream& in, // NOLINT
                              FeatureInfo* feature_info,
                              LabelInfo* label_info) {
  streampos first_object_pos = in.tellg();
  string object_str;
  int feature_count = 0;
  while (getline(in, object_str)) {
    try {
      double relevance, qid;
      vector<pair<int, double> > features;
      parseSVMLine(object_str, &relevance, &qid, &features);
      for (int feature_index = 0;
           feature_index < features.size();
           ++feature_index) {
        feature_count = std::max(feature_count, features[feature_index].first);
      }
    } catch(Parser::bad_line err) {}
  }

  *feature_info = FeatureInfo(feature_count);
  *label_info = LabelInfo();
  // IMPORTANT: now file is bad and we need to clear eofbit
  in.clear();
  in.seekg(first_object_pos);
}

void SVMParser::parseObject(const string& record,
                            const FeatureInfo& feature_info,
                            const LabelInfo& label_info,
                            Object* object) {
  double relevance, qid;
  vector<pair<int, double> > values;
  parseSVMLine(record, &relevance, &qid, &values);
  for (int value_index = 0; value_index < values.size(); ++value_index) {
    // Our object is 0-based so -1 is required
    object->at(values[value_index].first - 1) = values[value_index].second;
  }
  object->set_actual_label(relevance);
  if (!isNaN(qid)) {
    object->setMetaInfo("queryId", lexical_cast<string>(static_cast<int>(qid)));
  }
}

void SVMParser::saveObject(const Object& object, ostream& stream) { // NOLINT
  stream.precision(utility::DOUBLE_PRECISION);
  stream << object.actual_label() << " ";
  try {
    stream << "qid:" << object.getMetaInfo("queryId") << " ";
  } catch(logic_error err) { }
  for (int feature_index = 0;
       feature_index < object.feature_count();
       ++feature_index) {
    stream << feature_index+1 << ":"
        << object[feature_index] << " ";
  }
  stream << endl;
}

ListwiseDataSet
    SVMParser::buildListwiseDataSet(const std::vector<Object> &objects,
                                    const FeatureInfo& feature_info,
                                    const LabelInfo& label_info) {
  DataSet<ObjectList> data(feature_info, label_info);
  map<int, vector<Object> > objects_;
  typedef map<int, vector<Object> >::iterator object_iterator;

  groupByIntMeta(objects, "queryId", &objects_);

  for (object_iterator id_index = objects_.begin();
       id_index != objects_.end();
       ++id_index) {
    ObjectList list;
    for (int object_index = 0;
         object_index < (int)id_index->second.size();
         object_index++) {
      list << id_index->second[object_index];
    }
    data << list;
  }
  return data;
}

PairwiseDataSet
    SVMParser::buildPairwiseDataSet(const vector<Object>& objects,
                                    const FeatureInfo& feature_info,
                                    const LabelInfo& label_info) {
  DataSet<ObjectPair> data(feature_info, label_info);
  map<int, vector<Object> > objects_;
  typedef map<int, vector<Object> >::iterator object_iterator;

  groupByIntMeta(objects, "queryId", &objects_);

  for (object_iterator id_index = objects_.begin();
       id_index != objects_.end();
       ++id_index) {
    vector<Object> object_list = id_index->second;
    for (int first_object = 0;
         first_object < (int)object_list.size();
         ++first_object) {
      for (int second_object = 0;
           second_object < first_object;
           ++second_object) {
        data << ObjectPair(object_list[first_object],
                           object_list[second_object]);
      }
    }
  }
  return data;
}
};
};
