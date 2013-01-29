// Copyright 2011 Yandex

#include <stdexcept>
#include <sstream>

#include "ltr/data/utility/parsers/parse_arff.h"

#include "ltr/utility/numerical.h"
#include "ltr/utility/boost/lexical_cast.h"
#include "ltr/utility/boost/string_utils.h"

using ltr::utility::trim;
using ltr::utility::split;
using ltr::utility::to_upper;
using ltr::utility::lexical_cast;

using std::string;
using std::stringstream;
using std::map;

namespace ltr {
namespace io_utility {

  void ARFFParser::makeString(const Object& obj, std::string* result) {
    throw std::logic_error("it is impossible to save data in ARFF format");
  }

  PairwiseDataSet ARFFParser::buildPairwiseDataSet(
      const std::vector<Object>& objects,
      const FeatureInfo& info) {
    throw std::logic_error("can't build pairwise dataset for ARFF format");
  }

  ListwiseDataSet ARFFParser::buildListwiseDataSet(
      const std::vector<Object>& objects,
      const FeatureInfo& info) {
    throw std::logic_error("can't build listwise dataset for ARFF format");
  }

  void ARFFParser::parseRawObject(string line, RawObject* result) {
    if (line[0] == '%')
      throw Parser::bad_line();
    parseNextFeature.init(this);
    parseNextFeature.reset();

    trim(&line);
    vector<string> splitted;
    split(line, ",", &splitted);
    trim(&splitted);
    for (int value_num = 0; value_num < splitted.size(); ++value_num) {
      if (splitted[value_num][0] == '\'') {
        if (splitted[value_num][splitted[value_num].size() - 1] != '\'')
          throw std::logic_error("can't parse ARFF object: "
              "bad quoting");
        splitted[value_num] =
            splitted[value_num].substr(1, splitted[value_num].size());
      }
      parseNextFeature(splitted[value_num].c_str());
    }

    *result = features_;
  }

  void ARFFParser::init(std::istream *in) {
    std::string line;

    raw_feature_info_.clear();

    int feature_id = 1;
    int class_feature_id_ = -1;

    while (std::getline(*in, line)) {
      if (line.size() == 0 || line[0] != '@')
        continue;
      line = line.substr(1, line.size() - 1);
      trim(&line);
      string first;
      string other;
      vector<string> splitted;
      split(line, &splitted, 1);
      if (splitted.size() >= 1) first = splitted[0];
      if (splitted.size() >= 2) other = splitted[1];

      to_upper(&first);
      if (first == "DATA")
        break;

      if (first == "RELATION")
        continue;

      if (first != "ATTRIBUTE")
        throw std::logic_error("can't parse ARFF header");

      string attr_name;
      string attr_type;
      vector<string> values;

      split(other, &splitted, 1);
      trim(&splitted);
      if (splitted.size() != 2)
        throw std::logic_error("can't parse ARFF header: bad @ATTRIBUTE field");
      attr_name = splitted[0];
      attr_type = splitted[1];

      if (attr_type[0] == '{') {
        if (attr_type[attr_type.size() - 1] != '}')
          throw std::logic_error("can't parse ARFF header: "
              "bad nominal attribute");
        attr_type = attr_type.substr(1, attr_type.size() - 2);
        split(attr_type, ",", &values);
        trim(&values);
        for (int value_num = 0; value_num < values.size(); ++value_num) {
          if (values[value_num][0] == '\'') {
            if (values[value_num][values[value_num].size() - 1] != '\'')
              throw std::logic_error("can't parse ARFF header: "
                  "bad quoting");
            values[value_num] =
                values[value_num].substr(1, values[value_num].size());
          }
        }
      }

      to_upper(&attr_type);
      raw_feature_info_[feature_id].feature_name = attr_name;
      if (attr_name == "class") {
        raw_feature_info_[feature_id].feature_type = CLASS;
        for (int i = 0; i < (int)values.size(); ++i)
          classes_[values[i]] = i+1;
        class_feature_id_ = feature_id;
      } else if (attr_type == "NUMERIC" || attr_type == "REAL") {
        raw_feature_info_[feature_id].feature_type = NUMERIC;
      } else if (attr_type == "STRING") {
        raw_feature_info_[feature_id].feature_type = META;
      } else if (values.size() != 0) {
        raw_feature_info_[feature_id].feature_type = NOMINAL;
        raw_feature_info_[feature_id].feature_values = values;
      }
      feature_id++;
    }
    if (class_feature_id_ == -1)
      throw std::logic_error("no class attribute found");
}

  void ARFFParser::NextFeatureParser::reset() {
    parser_->current_id_ = 1;
    parser_->current_relevance_ = 0;
    parser_->features_.clear();
    parser_->meta_features_.clear();
  }

  void ARFFParser::NextFeatureParser::operator()(const char* feature_) const {
    std::string feature(feature_);
    int cur_id = parser_->current_id_;
    Parser::RawFeatureType type =
        parser_->raw_feature_info_[cur_id].feature_type;
    if (feature == "?") {
      parser_->current_id_++;
      return;
    }
    if (type == CLASS) {
      parser_->features_[parser_->current_id_++] =
        ltr::utility::lexical_cast<string>(parser_->classes_[feature]);
      return;
    }
    parser_->features_[parser_->current_id_++] = feature;
  }

  void ARFFParser::NextFeatureParser::init(ARFFParser* parser) {
    parser_ = parser;
  }
}
}
