// Copyright 2011 Yandex

#include <boost/algorithm/string.hpp>

#include "ltr/data/utility/parsers/object_parser.h"
#include "ltr/data/utility/parsers/parse_svm.h"
#include "ltr/data/utility/parsers/parse_yandex.h"
#include "ltr/data/utility/parsers/parse_arff.h"

namespace ltr {
  namespace io_utility {
    IParser::Ptr getParser(const std::string& format) {
      std::string format_ = format;
      boost::to_upper(format_);
      if (format_ == "SVMLITE")
        return IParser::Ptr(new SVMParser());
      else if (format_ == "YANDEX")
        return IParser::Ptr(new YandexParser());
      else if (format_ == "ARFF")
        return IParser::Ptr(new ARFFParser());
      else
        throw std::logic_error("unknown format " + format);
    }

    void IParser::writeString(const Object& obj, std::ostream* out) {
      std::string str;
      makeString(obj, &str);
      *out << str;
    }
  };
};
