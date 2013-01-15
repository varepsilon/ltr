// Copyright 2012 Yandex

#include <stdexcept>
#include <sstream>

#include "ltr/data/utility/parsers/parse_yandex.h"
#include "ltr/data/utility/io_utility.h"

#include "ltr/utility/numerical.h"
#include "ltr/utility/boost/string_utils.h"
#include "ltr/utility/boost/lexical_cast.h"

using ltr::utility::lexical_cast;
using ltr::utility::split;
using ltr::utility::trim_copy;

using std::string;
using std::stringstream;
using std::map;

namespace ltr {
namespace io_utility {
const int YandexParser::raw_query_id_idx_ = 0;
const int YandexParser::raw_relevance_idx_ = -1;

void YandexParser::init(std::istream* in) {
  raw_feature_info_[raw_query_id_idx_].feature_type = META;
  raw_feature_info_[raw_query_id_idx_].feature_name = "queryId";

  raw_feature_info_[raw_relevance_idx_].feature_type = CLASS;
  raw_feature_info_[raw_relevance_idx_].feature_name = "Label";
}

void YandexParser::parseRawObject(string line_, RawObject* result) {
  Object obj;
  string qid;
  RawObject& features = *result;
  int key;
  string relevance;
  try {
    string line = trim_copy(line_);
    vector<string> parse_data;
    split(line, "#", &parse_data);
    if (parse_data.size() > 2)
      throw std::logic_error("too much #");
    if (parse_data.size() == 0)
      throw std::logic_error("no data");

    if (parse_data.size() == 2) {
      qid = trim_copy(parse_data[1]);
      lexical_cast<int>(qid);
      parse_data.pop_back();
    }

    line = trim_copy(parse_data[0]);
    split(line, &parse_data);

    relevance = parse_data.front();
    lexical_cast<double>(relevance);

    for (int i = 1; i < parse_data.size(); ++i) {
      vector<string> feature;
      split(parse_data[i], ":", &feature);
      if (feature.size() != 2)
        throw logic_error("can't parse feature " + parse_data[i]);
      features[lexical_cast<int>(trim_copy(feature[0]))] =
          trim_copy(feature[1]);
    }
    features[raw_relevance_idx_] = relevance;
    if (!qid.empty())
      features[raw_query_id_idx_] = qid;
  } catch(std::exception e) {
    throw std::logic_error(
        "failed parse line " + line_ + " as Yandex (" + e.what() + ")");
  }
}

void YandexParser::makeString(const Object& object, string* result) {
  stringstream str;
  str.precision(utility::DOUBLE_PRECISION);
  str << object.actual_label() << " ";
  for (int feature_index = 0;
       feature_index < object.feature_count();
       feature_index++) {
    str << feature_index+1 << ":"
        << object[feature_index] << " ";
  }
  try {
    str << "# " << object.getMetaInfo("queryId");
  } catch(std::logic_error err) {}
  *result = str.str();
}

ListwiseDataSet YandexParser::buildListwiseDataSet(
  const std::vector<Object> &objects, const FeatureInfo& info) {
    DataSet<ObjectList> data(info);
    map<int, vector<Object> > objects_;
    typedef map<int, vector<Object> >::iterator object_iterator;

    groupByIntMeta(objects, "queryId", &objects_);

    for (object_iterator i = objects_.begin(); i != objects_.end(); ++i) {
      ObjectList list;
      for (int j = 0; j < (int)i->second.size(); j++)
        list << i->second[j];
      data << list;
    }
    return data;
  }

PairwiseDataSet YandexParser::buildPairwiseDataSet(
  const std::vector<Object> &objects, const FeatureInfo& info) {
    DataSet<ObjectPair> data(info);
    map<int, vector<Object> > objects_;
    typedef map<int, vector<Object> >::iterator object_iterator;

    groupByIntMeta(objects, "queryId", &objects_);

    for (object_iterator i = objects_.begin(); i != objects_.end(); ++i)
      for (int j = 0; j < (int)i->second.size(); j++)
        for (int k = 0; k < j; k++)
          data << ObjectPair(i->second[j], i->second[k]);
    return data;
}
};
};
