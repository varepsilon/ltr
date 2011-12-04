// Copyright 2011 Yandex

#include <boost/spirit/include/classic_push_back_actor.hpp>
#include <boost/spirit/include/classic_insert_at_actor.hpp>
#include <boost/spirit/include/classic_assign_actor.hpp>
#include <boost/algorithm/string/trim.hpp>

#include <stdexcept>
#include <sstream>

#include "ltr/data/utility/parsers/parse_svm.h"

#include "utility/numerical.h"

using boost::algorithm::trim_copy;
using boost::spirit::classic::parse;
using boost::spirit::classic::parse_info;
using boost::spirit::classic::real_p;
using boost::spirit::classic::ch_p;
using boost::spirit::classic::uint_p;
using boost::spirit::classic::space_p;
using boost::spirit::classic::anychar_p;
using boost::spirit::classic::assign_a;
using boost::spirit::classic::insert_at_a;
using boost::spirit::classic::str_p;

using std::string;
using std::stringstream;
using std::map;

namespace ltr {
  namespace io_utility {
  void SVMParser::parse(const std::string &line_,
                        double *relevance,
                        size_t *key,
                        std::map<size_t, double> *features,
                        size_t *qid) {
    string line = trim_copy(line_);
    parse_info<> info = boost::spirit::classic::parse(line.c_str(),
        (real_p[assign_a(*relevance)] >>
         str_p("qid") >> ch_p(':') >>
         uint_p[assign_a(*qid)] >>
         +(uint_p[assign_a(*key)] >>
          ch_p(':') >>
          real_p[insert_at_a(*features, *key)]) >>
         *anychar_p),
         space_p);
    if (!info.hit) {
      throw std::logic_error("failed parse line " + line + " as SVM");
    }
  }

  string SVMParser::makeString(const Object& obj) {
    stringstream str;
    str.precision(utility::DOUBLE_PRECISION);
    str << obj.actualLabel() << " qid:" << obj.queryId() << " ";
    for (size_t k = 0; k < obj.featureCount(); k++) {
      str << k+1 << ":" << obj.features()[k] << " ";
    }
    return str.str();
  }
  };
};
