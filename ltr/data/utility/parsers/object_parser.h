// Copyright 2011 Yandex

#ifndef LTR_DATA_UTILITY_PARSERS_OBJECT_PARSER_H_
#define LTR_DATA_UTILITY_PARSERS_OBJECT_PARSER_H_

#include <boost/spirit/include/classic_core.hpp>

#include <map>
#include <vector>
#include <string>
#include <iostream>

using std::vector;

#include "ltr/data/object.h"
#include "ltr/data/data_set.h"

#include "ltr/data/utility/nominal_feature_handler.h"

namespace ltr {
  namespace io_utility {
    class IParser {
      public:
        typedef boost::shared_ptr<IParser> Ptr;

        // function to init parser. For example read header.
        virtual void init(std::istream* in) {}
        virtual NominalFeatureInfo info() {return nominal_feature_info_;}

        // function to serialize Object to string
        virtual void makeString(const Object& obj, std::string* result) = 0;

        // function to write string to the stream
        virtual void writeString(const Object& obj, std::ostream* out);

        // function to parse string to object. if impossible, throws
        virtual Object parse(const std::string& line,
                             NominalFeatureHandler::Ptr featureHandler) = 0;

        virtual DataSet<ObjectPair> buildPairDataSet(
            const vector<Object>& objects, const FeatureInfo& info) = 0;

        virtual DataSet<ObjectList> buildListDataSet(
            const vector<Object>& objects, const FeatureInfo& info) = 0;

        virtual ~IParser() {}

      protected:
        NominalFeatureInfo nominal_feature_info_;
    };

    // returns parser for the given format
    IParser::Ptr getParser(const std::string& format);
  };
};

#endif  // LTR_DATA_UTILITY_PARSERS_OBJECT_PARSER_H_
