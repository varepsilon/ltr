// Copyright 2011 Yandex

#ifndef LTR_DATA_UTILITY_PARSERS_PARSE_ARFF_H_
#define LTR_DATA_UTILITY_PARSERS_PARSE_ARFF_H_

#include <vector>
#include <map>
#include <string>
#include <set>

#include "ltr/data/utility/parsers/parser.h"

using std::vector;
using std::set;

namespace ltr {
namespace io_utility {
  class ARFFParser : public Parser {
    private:
      void init(std::istream* in);

    public:
      virtual void parseRawObject(string line, RawObject* result);
      virtual void makeString(const Object& object, string* result);
      virtual void makeString(
          const FeatureInfo & feature_info,
          const vector<const Object *> & objects,
          std::string* result);

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
  };
};
};

#endif  // LTR_DATA_UTILITY_PARSERS_PARSE_ARFF_H_
