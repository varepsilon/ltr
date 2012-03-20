// Copyright 2011 Yandex

#ifndef LTR_DATA_UTILITY_PARSERS_PARSER_H_
#define LTR_DATA_UTILITY_PARSERS_PARSER_H_

#include <boost/spirit/include/classic_core.hpp>

#include <map>
#include <vector>
#include <string>
#include <iostream>

using std::vector;

#include "ltr/data/object.h"
#include "ltr/data/data_set.h"

namespace ltr {
  namespace io_utility {
    /**
     * @section DESCRIPTION
     * This class allows to parse strings into Objects
     *  and serialize Objects into strings.
     */
    class Parser {
      public:
        typedef int FeatureIndex;
        typedef int RawFeatureIndex;
        typedef string FeatureValue;
        typedef map<RawFeatureIndex, FeatureValue> RawObject;

        enum RawFeatureType {
          NUMERIC,
          NOMINAL,
          BOOLEAN,
          META,
          CLASS,
        };
        struct OneFeatureRawInfo {
          RawFeatureType feature_type;
          string feature_name;
          /**
           * List of possible feature values.
           * Contains nothing for NUMERIC and META features.
           */
          vector<string> feature_values;
        };

        typedef map<RawFeatureIndex, OneFeatureRawInfo> RawFeatureInfo;

        class bad_line : public std::logic_error {
          public:
            bad_line() : std::logic_error("") {}
        };
        typedef boost::shared_ptr<Parser> Ptr;

        /**
         * Function sets stream to parse objects from and initialize parser. 
         */
        void setStream(std::istream* in);

        /**
         * Function returns FeatureInfo for the parsed DataSet.
         */
        const FeatureInfo& featureInfo() {return feature_info_;}

        /**
         * Function to parse next object from stream.
         * @return 1 if parsed and 0 if has no more objects in stream.
         * @param result - Link to the Object to save parsed object.
         */
        int parseNextObject(Object* result);

        /**
         * Function to parse string into RawObject.
         * Can throw Parser::bad_line if impossible to parse.
         * @param line - string to parse.
         * @param result - link to RawObject to save result in.
         */
        virtual void parseRawObject(string line, RawObject* result) = 0;

        /**
         * Function to convert RawObject into Object.
         * It sets actualLabel, meta info
         * and converts nominal feature into double values.
         * @return Converted object
         * @param raw_object - RawObject to convert.
         */
        Object makeObject(const RawObject& raw_object);
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

        virtual ~Parser() {}

      protected:
        map<RawFeatureIndex, FeatureIndex> feature_id_;
        RawFeatureInfo raw_feature_info_;
        int last_feature_idx_;
        FeatureInfo feature_info_;

        /**
         * Stream to parse objects from.
         */
        std::istream* file_;

        /**
         * Method to init parser. It must fill the raw_feature_onfo_.
         * @code
         * ifstream fin("file.txt");
         * parser->init(&fin);
         * @endcode
         */
        virtual void init(std::istream* in) {}

        double hash(const string str) {
          return 0;
        }
    };

    /**
     * Returns the parser for given format.
     * @param format - format to create parser for.
     * @code
     * Parser::Ptr yandex_parser = getParser("yandex");
     * @endcode
     */
    Parser::Ptr getParser(const std::string& format);
  };
};

#endif  // LTR_DATA_UTILITY_PARSERS_PARSER_H_
