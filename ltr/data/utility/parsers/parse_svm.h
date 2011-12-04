// Copyright 2011 Yandex

#ifndef LTR_DATA_UTILITY_PARSERS_PARSE_SVM_H_
#define LTR_DATA_UTILITY_PARSERS_PARSE_SVM_H_

#include <boost/spirit/include/classic_core.hpp>

#include <map>
#include <string>
#include <iostream>

#include "ltr/data/object.h"
#include "ltr/data/object_pair.h"
#include "ltr/data/object_list.h"

#include "ltr/data/utility/parsers/object_parser.h"

namespace ltr {
  namespace io_utility {
  class SVMParser : public IParser {
    public:
      void parse(const std::string& line,
                 double* relevance,
                 size_t* key,
                 std::map<size_t, double>* features,
                 size_t* qid);
      void parseToObject(const std::string line, Object* obj);
      std::string makeString(const Object& obj);
  };
  };
};

#endif  // LTR_DATA_UTILITY_PARSERS_PARSE_SVM_H_
