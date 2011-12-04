// Copyright 2011 Yandex

#include <boost/algorithm/string.hpp>

#include "ltr/data/utility/parsers/object_parser.h"
#include "ltr/data/utility/parsers/parse_svm.h"
#include "ltr/data/utility/parsers/parse_yandex.h"

namespace ltr {
  namespace io_utility {
    IParser::Ptr getParser(const std::string& format) {
      std::string format_ = format;
      boost::to_upper(format_);
      if (format_ == "SVMLITE")
        return IParser::Ptr(new SVMParser());
      else if (format_ == "YANDEX")
        return IParser::Ptr(new YandexParser());
      else
        throw std::logic_error("unknown format " + format);
    }

    Object IParser::parseToObject(const std::string line) {
      Object obj;
      double relevance = 0;
      size_t key, qid;
      std::map<size_t, double> features;
      parse(line, &relevance, &key, &features, &qid);

      obj.setActualLabel(relevance);
      obj.setQuieryId(qid);

      obj.features().assign(features.rbegin()->first, 0.0);

      for (std::map<size_t, double>::const_iterator it = features.begin();
              it != features.end(); ++it) {
          obj.features()[it->first-1] = it->second;
      }
      return obj;
    }

    void IParser::writeString(const Object& obj, std::ostream* out) {
      *out << makeString(obj);
    }
  };
};
