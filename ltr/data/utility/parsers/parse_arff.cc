// Copyright 2011 Yandex

#include <boost/spirit/include/classic_push_back_actor.hpp>
#include <boost/spirit/include/classic_insert_at_actor.hpp>
#include <boost/spirit/include/classic_assign_actor.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/trim.hpp>

#include <stdexcept>
#include <sstream>

#include "ltr/data/utility/parsers/parse_arff.h"

#include "ltr/utility/numerical.h"
#include "ltr/utility/boost/lexical_cast.h"

using boost::algorithm::trim_copy;
using boost::spirit::classic::parse;
using boost::spirit::classic::parse_info;
using boost::spirit::classic::real_p;
using boost::spirit::classic::ch_p;
using boost::spirit::classic::uint_p;
using boost::spirit::classic::space_p;
using boost::spirit::classic::digit_p;
using boost::spirit::classic::anychar_p;
using boost::spirit::classic::alnum_p;
using boost::spirit::classic::assign_a;
using boost::spirit::classic::insert_at_a;
using boost::spirit::classic::str_p;
using boost::spirit::classic::lexeme_d;
using boost::spirit::classic::anychar_p;
using boost::spirit::classic::push_back_a;
using boost::spirit::classic::rule;
using ltr::utility::lexical_cast;

using std::string;
using std::stringstream;
using std::map;

namespace ltr {
namespace io_utility {

  void ARFFParser::makeString(const Object& obj, std::string* result) {
    throw std::logic_error("it is impossible to save data in ARFF format");
  }

  PairwiseDataSet ARFFParser::buildPairwiseDataSet(
      const std::vector<Object>& objects,
      const FeatureInfo& info) {
    throw std::logic_error("can't build pairwise dataset for ARFF format");
  }

  ListwiseDataSet ARFFParser::buildListwiseDataSet(
      const std::vector<Object>& objects,
      const FeatureInfo& info) {
    throw std::logic_error("can't build listwise dataset for ARFF format");
  }

  void ARFFParser::parseRawObject(string line, RawObject* result) {
    if (line[0] == '%')
      throw Parser::bad_line();
    parseNextFeature.init(this);
    parseNextFeature.reset();

    rule<> number = !ch_p('-') >> +digit_p >> !('.' >> *digit_p);
    rule<> nan = lexeme_d[str_p("?")];
    rule<> string_rule = lexeme_d[*(anychar_p - ch_p('\''))];
    rule<> simple_string = lexeme_d[*(alnum_p | '-')];
    rule<> quoter = ch_p('\'');

    string line_ = trim_copy(line);
    parse_info<> info = boost::spirit::classic::parse(line_.c_str(),
      ((nan | number | simple_string)[parseNextFeature] |
        (quoter >> string_rule[parseNextFeature] >> quoter))
       % ','
      , space_p);

    if (!info.hit)
      throw std::logic_error("can't parse ARFF string");
    *result = features_;
  }

  void ARFFParser::init(std::istream *in) {
    std::string line;

    raw_feature_info_.clear();

    int feature_id = 1;
    int class_feature_id_ = -1;

    while (std::getline(*in, line)) {
      if (line.size() == 0 || line[0] != '@')
        continue;
      line = line.substr(1, line.size() - 1);
      boost::algorithm::trim(line);
      string first;
      string other;
      parse_info<> info = boost::spirit::classic::parse(line.c_str(),
        lexeme_d[+alnum_p][assign_a(first)] >> *space_p >>
        lexeme_d[*anychar_p][assign_a(other)]);

      if (!info.hit)
        throw std::logic_error("can't parse ARFF header");

      boost::to_upper(first);
      if (first == "DATA")
        break;

      if (first == "RELATION")
        continue;

      if (first != "ATTRIBUTE")
        throw std::logic_error("can't parse ARFF header");

      string attr_name;
      string attr_type;
      vector<string> values;

      rule<> string_rule = lexeme_d[*(anychar_p - ch_p('\''))];
      rule<> simple_string = lexeme_d[*(alnum_p | '-')];
      rule<> quoter = ch_p('\'');

      info = boost::spirit::classic::parse(other.c_str(),
        lexeme_d[+(alnum_p | '_' | '-')][assign_a(attr_name)] >> +space_p >>
        (lexeme_d[+alnum_p][assign_a(attr_type)] |
        ('{' >> (( (quoter >> string_rule[push_back_a(values)] >> quoter) |
                  simple_string[push_back_a(values)]) % ',')
             >> '}') ));
      if (!info.hit)
        throw std::logic_error("can't parse ARFF header");

      boost::to_upper(attr_type);
      raw_feature_info_[feature_id].feature_name = attr_name;
      if (attr_name == "class") {
        raw_feature_info_[feature_id].feature_type = CLASS;
        for (int i = 0; i < (int)values.size(); ++i)
          classes_[values[i]] = i+1;
        class_feature_id_ = feature_id;
      } else if (attr_type == "NUMERIC" || attr_type == "REAL") {
        raw_feature_info_[feature_id].feature_type = NUMERIC;
      } else if (attr_type == "STRING") {
        raw_feature_info_[feature_id].feature_type = META;
      } else if (values.size() != 0) {
        raw_feature_info_[feature_id].feature_type = NOMINAL;
        raw_feature_info_[feature_id].feature_values = values;
      }
      feature_id++;
    }
    if (class_feature_id_ == -1)
      throw std::logic_error("no class attribute found");
}

  void ARFFParser::NextFeatureParser::reset() {
    parser_->current_id_ = 1;
    parser_->current_relevance_ = 0;
    parser_->features_.clear();
    parser_->meta_features_.clear();
  }

  void ARFFParser::NextFeatureParser::operator()(const char* feature_,
                                                 const char* it) const {
    std::string feature(feature_, it);
    int cur_id = parser_->current_id_;
    Parser::RawFeatureType type =
        parser_->raw_feature_info_[cur_id].feature_type;
    if (feature == "?") {
      parser_->current_id_++;
      return;
    }
    if (type == CLASS) {
      parser_->features_[parser_->current_id_++] =
        ltr::utility::lexical_cast<string>(parser_->classes_[feature]);
      return;
    }
    parser_->features_[parser_->current_id_++] = feature;
  }

  void ARFFParser::NextFeatureParser::init(ARFFParser* parser) {
    parser_ = parser;
  }
}
}
