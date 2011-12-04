// Copyright 2011 Yandex

#ifndef LTR_DATA_UTILITY_PARSERS_OBJECT_PARSER_H_
#define LTR_DATA_UTILITY_PARSERS_OBJECT_PARSER_H_

#include <boost/spirit/include/classic_core.hpp>

#include <map>
#include <string>
#include <iostream>

#include "ltr/data/object.h"
#include "ltr/data/data_set.h"

namespace ltr {
  namespace io_utility {
    class IParser {
      public:
        typedef boost::shared_ptr<IParser> Ptr;
        // function to init parser. For example read header.
        virtual void init(std::istream* in) {}
        // function to write file header
        virtual void writeHeader(const FeatureInfo& info) {}
        // function to serialize Object to string
        virtual std::string makeString(const Object& obj) = 0;
        // function to write string to the stream
        virtual void writeString(const Object& obj, std::ostream* out);
        // function to parse string to object. if impossible, throws
        virtual void parse(const std::string& line,
                           double* relevance,
                           size_t* key,
                           std::map<size_t, double>* features,
                           size_t* qid) = 0;
        virtual Object parseToObject(const std::string line);

        virtual ~IParser() {}
    };

    // returns parser for the given format
    IParser::Ptr getParser(const std::string& format);
  };
};

#endif  // LTR_DATA_UTILITY_PARSERS_OBJECT_PARSER_H_
