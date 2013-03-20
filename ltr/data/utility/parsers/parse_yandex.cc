// Copyright 2013 Yandex

#include <sstream>
#include <stdexcept>
#include <utility>

#include "ltr/data/utility/parsers/parse_yandex.h"
#include "ltr/data/utility/io_utility.h"
#include "ltr/utility/numerical.h"
#include "ltr/utility/boost/string_utils.h"
#include "ltr/utility/boost/lexical_cast.h"

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
using std::max;
using std::pair;
using std::streampos;
using std::string;
using std::stringstream;
using std::vector;

namespace ltr {
namespace io_utility {
void parseYandexLine(const string& line, double* relevance, double* qid,
                     vector<pair<int, double> > *feature_values) {
  *qid = NaN;
  feature_values->clear();
  vector<string> parse_data;
  split(trim_copy(line), "#", &parse_data);
  if (parse_data.size() > 2) {
    throw logic_error("too much #");
  }
  if (parse_data.size() == 0) {
    throw logic_error("no feature_values");
  }
  trim(&parse_data);
  if (parse_data[0].size() == 0) {
    throw Parser::bad_line();
  }

  if (parse_data.size() == 2) {
    *qid = lexical_cast<int>(parse_data[1]);
  }

  string features_line = parse_data[0];
  split(features_line, &parse_data);

  *relevance = lexical_cast<double>(parse_data.front());

  for (int value_index = 1; value_index < parse_data.size(); ++value_index) {
    vector<string> feature;
    split(parse_data[value_index], ":", &feature);
    if (feature.size() == 2) {
      feature_values->push_back(make_pair(lexical_cast<int>(feature[0]),
                                          lexical_cast<double>(feature[1])));

    } else {
      throw logic_error("can't parse feature " + parse_data[value_index]);
    }
  }
}

void YandexParser::parseDataInfo(istream& in, // NOLINT
                                 FeatureInfo* feature_info,
                                 LabelInfo* label_info) {
  streampos first_object_pos = in.tellg();
  string object_str;
  int feature_count = 0;
  while (getline(in, object_str)) {
    try {
      double relevance, qid;
      vector<pair<int, double> > features;
      parseYandexLine(object_str, &relevance, &qid, &features);
      for (int feature_index = 0;
           feature_index < features.size();
           ++feature_index) {
        feature_count = max(feature_count, features[feature_index].first);
      }
    } catch(Parser::bad_line err) {}
  }

  *feature_info = FeatureInfo(feature_count);
  *label_info = LabelInfo();
  // IMPORTANT: now file is bad and we need to clear eofbit
  in.clear();
  in.seekg(first_object_pos);
}

void YandexParser::parseObject(const string& record,
                               const FeatureInfo& feature_info,
                               const LabelInfo& label_info,
                               Object* object) {
  double relevance, qid;
  vector<pair<int, double> > features;
  parseYandexLine(record, &relevance, &qid, &features);
  for (int value_index = 0; value_index < features.size(); ++value_index) {
    // Our object is 0-based so -1 is required
    object->at(features[value_index].first - 1) = features[value_index].second;
  }
  object->set_actual_label(relevance);
  if (!isNaN(qid)) {
    object->setMetaInfo("queryId", lexical_cast<string>(static_cast<int>(qid)));
  }
}

void YandexParser::saveObject(const Object& object, ostream& stream) { // NOLINT
  stream.precision(utility::DOUBLE_PRECISION);
  stream << object.actual_label() << " ";
  for (int feature_index = 0;
       feature_index < object.feature_count();
       ++feature_index) {
    stream << feature_index+1 << ":"
        << object[feature_index] << " ";
  }
  try {
    stream << "# " << object.getMetaInfo("queryId");
  } catch(logic_error err) { }
  stream << endl;
}

ListwiseDataSet
    YandexParser::buildListwiseDataSet(const vector<Object> &objects,
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
    YandexParser::buildPairwiseDataSet(const vector<Object>& objects,
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
