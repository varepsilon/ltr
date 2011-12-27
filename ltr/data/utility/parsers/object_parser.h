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
    /**
     * @section DESCRIPTION
     * This class allows to parse strings into Objects
     *  and serialize Objects into strings.
     */
    class IParser {
      public:
        class comment : public std::logic_error {
          public:
            comment() : std::logic_error("") {}
        };
        typedef boost::shared_ptr<IParser> Ptr;

        /**
         * Method to init parser. For example, read file header.
         * @code
         * ifstream fin("file.txt");
         * parser->init(&fin);
         * @endcode
         */
        virtual void init(std::istream* in) {}
        /**
         * Function, returning information about features in file.
         */
        virtual NominalFeatureInfo info() {return nominal_feature_info_;}

        /**
         * Functions serializes object into string.
         * @param object - object to serialize
         * @param result - string, which will contain serialized object
         * @code
         * string result;
         * parser->makeString(object, &result);
         * @endcoe
         */
        virtual void makeString(const Object& object, std::string* result) = 0;

        /**
         * Method to write serialized object into stream.
         * @code
         * ofstream fout("file.txt");
         * parser->writeString(object, &fout);
         * @endcode
         */
        virtual void writeString(const Object& obj, std::ostream* out);

        /**
         * Function to parse string into object.
         * @param line -string to parse
         * @param featureHandler - class,
         *  converting features into vector of numbers
         */
        virtual Object parse(const std::string& line,
                             NominalFeatureHandler::Ptr featureHandler) = 0;
        /**
         * Function to create Pairwise data set of given objects
         * @param objects - objects to create data set from
         * @param info - information about features in the object
         */
        virtual DataSet<ObjectPair> buildPairDataSet(
            const vector<Object>& objects, const FeatureInfo& info) = 0;
        /**
         * Function to create Listwise data set of given objects
         * @param objects - objects to create data set from
         * @param info - information about features in the object
         */
        virtual DataSet<ObjectList> buildListDataSet(
            const vector<Object>& objects, const FeatureInfo& info) = 0;

        virtual ~IParser() {}

      protected:
        NominalFeatureInfo nominal_feature_info_;
    };

    /**
     * Returns the parser for given format.
     * @param format - format to create parser for.
     * @code
     * IParser::Ptr yandex_parser = getParser("yandex");
     * @endcode
     */
    IParser::Ptr getParser(const std::string& format);
  };
};

#endif  // LTR_DATA_UTILITY_PARSERS_OBJECT_PARSER_H_
