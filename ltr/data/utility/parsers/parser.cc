// Copyright 2011 Yandex

#include "ltr/data/utility/parsers/parser.h"
#include "ltr/data/utility/parsers/parse_svm.h"
#include "ltr/data/utility/parsers/parse_yandex.h"
#include "ltr/data/utility/parsers/parse_arff.h"

#include "ltr/utility/numerical.h"
#include "ltr/utility/boost/string_utils.h"

namespace ltr {
namespace io_utility {
Parser::Ptr getParser(const std::string& format) {
  std::string format_ = format;
  ltr::utility::to_upper(&format_);
  if (format_ == "SVMLIGHT")
    return Parser::Ptr(new SVMParser());
  else if (format_ == "YANDEX")
    return Parser::Ptr(new YandexParser());
  else if (format_ == "ARFF")
    return Parser::Ptr(new ARFFParser());
  else
    throw std::logic_error("unknown format " + format);
}

void Parser::startParsing(std::istream* in) {
  file_ = in;
  init(in);

  last_feature_index_ = -1;
  NominalFeatureValues values;
  for (RawFeatureInfo::iterator iterator = raw_feature_info_.begin();
       iterator != raw_feature_info_.end(); ++iterator) {
    RawFeatureIndex raw_idx = iterator->first;
    values.clear();
    switch (raw_feature_info_[raw_idx].feature_type) {
    case NOMINAL: {
      feature_id_[raw_idx] = ++last_feature_index_;
      vector<string>& vals = raw_feature_info_[raw_idx].feature_values;
      for (int index = 0; index < (int)vals.size(); ++index)
        values[hash(vals[index])] = vals[index];
      feature_info_.addFeature(ltr::NOMINAL, values);
    } break;
    case BOOLEAN: {
          feature_id_[raw_idx] = ++last_feature_index_;
          values[0] = "false";
          values[1] = "true";
          feature_info_.addFeature(ltr::BOOLEAN, values);
    } break;
    case NUMERIC: {
      feature_id_[raw_idx] = ++last_feature_index_;
      feature_info_.addFeature(ltr::NUMERIC);
    } break;
    case META: break;
    case CLASS: break;
    default: throw std::logic_error("Unknown raw feature type");
    }
  }
}

int Parser::parseNextObject(ltr::Object* result) {
  if (!file_)
    throw std::logic_error("error: parser isn't inited with a stream.");
  string line;
  RawObject raw_object;

  while (std::getline(*file_, line))
    try {
      parseRawObject(line, &raw_object);
      *result = makeObject(raw_object);
      return 1;
    } catch(Parser::bad_line err) {}
  return 0;
}

Object Parser::makeObject(const RawObject& raw_object) {
  if (raw_feature_info_.rbegin()->first < raw_object.rbegin()->first) {
    for (int index = raw_feature_info_.rbegin()->first + 1;
         index <= raw_object.rbegin()->first; ++index) {
      feature_id_[index] = ++last_feature_index_;
      raw_feature_info_[index].feature_type = NUMERIC;
    }
    feature_info_.resize(raw_object.rbegin()->first,
                         ltr::NUMERIC);
  }

  Object result;
  result.features().resize(feature_info_.feature_count());

  for (RawObject::const_iterator iterator = raw_object.begin();
       iterator != raw_object.end(); ++iterator) {
    RawFeatureIndex raw_idx = iterator->first;
    int feature_index = feature_id_[raw_idx];
    try {
      switch (raw_feature_info_[raw_idx].feature_type) {
      case NOMINAL: result[feature_index] = hash(iterator->second); break;
      case BOOLEAN: break;
      case NUMERIC:
        result[feature_index] =
            ltr::utility::lexical_cast<double>(iterator->second);
        break;
      case META:
        result.setMetaInfo(raw_feature_info_[raw_idx].feature_name,
                           iterator->second);
        break;
      case CLASS:
        result.set_actual_label(
            ltr::utility::lexical_cast<double>(iterator->second));
        break;
      default: throw std::logic_error("Unknown raw feature type");
      }
    } catch(ltr::utility::bad_lexical_cast err) {
      throw std::logic_error("can't parse " + iterator->second + " as double");
    }
  }

  return result;
}

void Parser::makeString(
    const FeatureInfo & feature_info,
    const vector<const Object *> & objects,
    std::string* result) {
  stringstream result_stream;
  for (int object_index = 0;
       object_index < objects.size();
       ++object_index) {
    string str;
    makeString(*(objects[object_index]), &str);
    result_stream << str;
    if (object_index + 1 < objects.size())
      result_stream << std::endl;
  }
  *result = result_stream.str();    
}

};
};
