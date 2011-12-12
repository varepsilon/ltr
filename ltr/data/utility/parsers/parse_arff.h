// Copyright 2011 Yandex

#ifndef LTR_DATA_UTILITY_PARSERS_PARSE_ARFF_H_
#define LTR_DATA_UTILITY_PARSERS_PARSE_ARFF_H_

#include <boost/spirit/include/classic_parser.hpp>

#include <vector>
#include <map>
#include <string>
#include <set>

#include "ltr/data/utility/parsers/object_parser.h"

using boost::spirit::classic::parser;
using std::vector;
using std::set;

namespace ltr {
namespace io_utility {
  class ARFFParser : public IParser {
    public:
      void init(std::istream* in);
      Object parse(const std::string& line,
                   NominalFeatureHandler::Ptr featureHandler);
      void makeString(const Object& obj, std::string* result);

      DataSet<ObjectPair> buildPairDataSet(const std::vector<Object>& objects,
                                           const FeatureInfo& info);
      DataSet<ObjectList> buildListDataSet(const std::vector<Object>& objects,
                                           const FeatureInfo& info);

    private:
      int current_id_;
      int current_out_id_;
      map<int, string> features_;
      map<string, string> meta_features_;
      class NextFeatureParser {
        public:
          void init(ARFFParser* parser);
          void operator()(const char* feature,
                          const char* it) const;
          void reset();

        private:
          NominalFeatureInfo info_;
          ARFFParser* parser_;
      } parseNextFeature;
      friend class NextFeatureParser;
  };
};
};

#endif  // LTR_DATA_UTILITY_PARSERS_PARSE_ARFF_H_
