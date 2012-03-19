// Copyright 2011 Yandex

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include "ltr/data/utility/parsers/parser.h"
#include "ltr/data/utility/parsers/parse_svm.h"
#include "ltr/data/utility/parsers/parse_yandex.h"
#include "ltr/data/utility/parsers/parse_arff.h"

#include "ltr/utility/numerical.h"

namespace ltr {
  namespace io_utility {
    Parser::Ptr getParser(const std::string& format) {
      std::string format_ = format;
      boost::to_upper(format_);
      if (format_ == "SVMLITE")
        return Parser::Ptr(new SVMParser());
      else if (format_ == "YANDEX")
        return Parser::Ptr(new YandexParser());
      else if (format_ == "ARFF")
        return Parser::Ptr(new ARFFParser());
      else
        throw std::logic_error("unknown format " + format);
    }

    void Parser::writeString(const Object& obj, std::ostream* out) {
      std::string str;
      makeString(obj, &str);
      *out << str;
    }

    void Parser::setStream(std::istream* in) {
      file_ = in;
      init(in);

      last_feature_idx_ = -1;
      NominalFeatureValues values;
      for (RawFeatureInfo::iterator it = raw_feature_info_.begin();
           it != raw_feature_info_.end(); it++) {
        RawFeatureIndex raw_idx = it->first;
        values.clear();
        switch (raw_feature_info_[raw_idx].feature_type) {
          case NOMINAL: {
              feature_id_[raw_idx] = ++last_feature_idx_;
              vector<string>& vals = raw_feature_info_[raw_idx].feature_values;
              for (size_t i = 0; i < vals.size(); i++)
                values[hash(vals[i])] = vals[i];
              feature_info_.addFeature(ltr::NOMINAL, values);
            }
            break;
          case BOOLEAN:
            feature_id_[raw_idx] = ++last_feature_idx_;
            values[0] = "false";
            values[1] = "true";
            feature_info_.addFeature(ltr::BOOLEAN, values);
            break;
          case NUMERIC:
            feature_id_[raw_idx] = ++last_feature_idx_;
            feature_info_.addFeature(ltr::NUMERIC);
            break;
          case META:
          case CLASS:
            break;
          default: throw std::logic_error("Unknown raw feature type");
        }
      }
    }

    int Parser::parseNextObject(ltr::Object *result) {
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
      Object result;

      if (raw_feature_info_.rbegin()->first < raw_object.rbegin()->first) {
        for (int i = raw_feature_info_.rbegin()->first + 1;
             i <= raw_object.rbegin()->first; i++) {
          feature_id_[i] = ++last_feature_idx_;
          raw_feature_info_[i].feature_type = NUMERIC;
        }
        feature_info_.setFeatureCount(raw_object.rbegin()->first,
                                      ltr::NUMERIC);
      }

      result.features().resize(feature_info_.getFeatureCount(), utility::NaN);

      for (RawObject::const_iterator it = raw_object.begin();
           it != raw_object.end(); it++) {
        RawFeatureIndex raw_idx = it->first;
        int feature_idx = feature_id_[raw_idx];
        try {
          switch (raw_feature_info_[raw_idx].feature_type) {
            case NOMINAL:
              result.features()[feature_idx] = hash(it->second);
              break;
            case BOOLEAN:
            case NUMERIC:
              result.features()[feature_idx] =
                boost::lexical_cast<double>(it->second);
              break;
            case META:
              result.metaInfo()[raw_feature_info_[raw_idx].feature_name] =
                it->second;
              break;
            case CLASS:
              result.setActualLabel(boost::lexical_cast<double>(it->second));
              break;
            default: throw std::logic_error("Unknown raw feature type");
          }
        } catch(boost::bad_lexical_cast err) {
          throw std::logic_error("can't parse " + it->second + " as double");
        }
      }

      return result;
    }
  };
};
