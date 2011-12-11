// Copyright 2011 Yandex

#ifndef LTR_DATA_UTILITY_PARSERS_PARSE_SVM_H_
#define LTR_DATA_UTILITY_PARSERS_PARSE_SVM_H_

#include <boost/spirit/include/classic_core.hpp>

#include <map>
#include <string>
#include <vector>
#include <iostream>

#include "ltr/data/object.h"
#include "ltr/data/object_pair.h"
#include "ltr/data/object_list.h"

#include "ltr/data/utility/parsers/object_parser.h"

namespace ltr {
  namespace io_utility {
  class SVMParser : public IParser {
    public:
      Object parse(const std::string& line,
                   NominalFeatureHandler::Ptr featureHandler);
      void makeString(const Object& obj, std::string* result);

      DataSet<ObjectPair> buildPairDataSet(const std::vector<Object>& objects,
                                           const FeatureInfo& info);
      DataSet<ObjectList> buildListDataSet(const std::vector<Object>& objects,
                                           const FeatureInfo& info);
  };
  };
};

#endif  // LTR_DATA_UTILITY_PARSERS_PARSE_SVM_H_
