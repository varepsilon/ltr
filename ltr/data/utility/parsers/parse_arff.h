// Copyright 2011 Yandex

#ifndef LTR_DATA_UTILITY_PARSERS_PARSE_ARFF_H_
#define LTR_DATA_UTILITY_PARSERS_PARSE_ARFF_H_

#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_parser.hpp>

#include <vector>
#include <map>
#include <string>
#include <set>

#include "ltr/data/utility/parsers/parser.h"

using boost::spirit::classic::parser;
using std::vector;
using std::set;

namespace ltr {
namespace io_utility {
  class ARFFParser : public Parser {
    private:
      void init(std::istream* in);

    public:
      virtual void parseRawObject(string line, RawObject* result);
      void makeString(const Object& obj, std::string* result);

      PairwiseDataSet buildPairwiseDataSet(const std::vector<Object>& objects,
                                           const FeatureInfo& info);
      ListwiseDataSet buildListwiseDataSet(const std::vector<Object>& objects,
                                           const FeatureInfo& info);

    private:
      int current_id_;
      double current_relevance_;
      map<int, string> features_;
      map<string, double> classes_;
      map<string, string> meta_features_;
      /**
       * @class NextFeatureParser
       * This class is used only by ARFF Parser. It is functor,
       * which is used in callbacks from boost::spirit
       */
      class NextFeatureParser {
        public:
          /**
           * Function inits NextFatureParser.
           */
          void init(ARFFParser* parser);
          /**
           * This operator is called by boost::spirit.
           * It processes features and saves it in the map in the parser,
           * given in init.
           */
          void operator()(const char* feature,
                          const char* it) const;
          /**
           * Function prepares NextFeatureParser to process the next object.
           */
          void reset();

        private:
          ARFFParser* parser_;
      } parseNextFeature;
      friend class NextFeatureParser;
  };
};
};

#endif  // LTR_DATA_UTILITY_PARSERS_PARSE_ARFF_H_
