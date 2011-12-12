// Copyright 2011 Yandex

#include <boost/spirit/include/classic_push_back_actor.hpp>
#include <boost/spirit/include/classic_insert_at_actor.hpp>
#include <boost/spirit/include/classic_assign_actor.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/lexical_cast.hpp>

#include <stdexcept>
#include <sstream>

#include "ltr/data/utility/parsers/parse_svm.h"
#include "ltr/data/utility/data_set_utility.h"
#include "utility/numerical.h"

using boost::algorithm::trim_copy;
using boost::spirit::classic::parse;
using boost::spirit::classic::parse_info;
using boost::spirit::classic::real_p;
using boost::spirit::classic::ch_p;
using boost::spirit::classic::uint_p;
using boost::spirit::classic::space_p;
using boost::spirit::classic::anychar_p;
using boost::spirit::classic::alnum_p;
using boost::spirit::classic::digit_p;
using boost::spirit::classic::assign_a;
using boost::spirit::classic::insert_at_a;
using boost::spirit::classic::str_p;
using boost::spirit::classic::lexeme_d;
using boost::lexical_cast;

using std::string;
using std::stringstream;
using std::map;

namespace ltr {
  namespace io_utility {
  Object SVMParser::parse(const std::string &line_,
                          NominalFeatureHandler::Ptr feature_handler) {
    Object obj;
    int qid = -1;
    map<int, string> features;
    int key;
    double relevance;
    string line = trim_copy(line_);
    parse_info<> info = boost::spirit::classic::parse(line.c_str(),
        (real_p[assign_a(relevance)] >>
         str_p("qid") >> ':' >> uint_p[assign_a(qid)] >>
         +(uint_p[assign_a(key)] >> ':' >>
          lexeme_d[+digit_p >> !('.' >> +digit_p)][insert_at_a(features, key)])
          >> *anychar_p),
         space_p);
    if (!info.hit) {
      throw std::logic_error("failed parse line " + line + " as SVM");
    }
    obj.setActualLabel(relevance);
    feature_handler->process(features, &obj.features());
    if (qid > 0)
      obj.setMetaInfo("queryId", lexical_cast<string>(qid));
    return obj;
  }

  void SVMParser::makeString(const Object& obj, string*result) {
    stringstream str;
    str.precision(utility::DOUBLE_PRECISION);
    str << obj.actualLabel() << " ";
    try {
      str << "qid:" << obj.getMetaInfo("queryId") << " ";
    } catch(std::logic_error err) { }
    for (size_t k = 0; k < obj.featureCount(); k++) {
      str << k+1 << ":" << obj.features()[k] << " ";
    }
    *result = str.str();
  }

  DataSet<ObjectList> SVMParser::buildListDataSet(
      const std::vector<Object> &objects, const FeatureInfo& info) {
    DataSet<ObjectList> data(info);
    map<int, vector<Object> > objects_;
    typedef map<int, vector<Object> >::iterator object_iterator;

    utility::groupByIntMeta(objects, "queryId", &objects_);

    for (object_iterator i = objects_.begin(); i != objects_.end(); i++) {
      ObjectList list;
      for (int j = 0; j < i->second.size(); j++)
        list << i->second[j];
      data << list;
    }
    return data;
  }

  DataSet<ObjectPair> SVMParser::buildPairDataSet(
      const std::vector<Object> &objects, const FeatureInfo& info) {
    DataSet<ObjectPair> data(info);
    map<int, vector<Object> > objects_;
    typedef map<int, vector<Object> >::iterator object_iterator;

    utility::groupByIntMeta(objects, "queryId", &objects_);

    for (object_iterator i = objects_.begin(); i != objects_.end(); i++)
      for (int j = 0; j < i->second.size(); j++)
        for (int k = 0; k < j; k++)
          data << ObjectPair(i->second[j], i->second[k]);
    return data;
  }
  };
};
