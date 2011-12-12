// Copyright 2011 Yandex

#include <boost/spirit/include/classic_push_back_actor.hpp>
#include <boost/spirit/include/classic_insert_at_actor.hpp>
#include <boost/spirit/include/classic_assign_actor.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/trim.hpp>

#include <stdexcept>
#include <sstream>

#include "ltr/data/utility/parsers/parse_arff.h"

#include "utility/numerical.h"

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
using boost::lexical_cast;

using std::string;
using std::stringstream;
using std::map;

namespace ltr {
namespace io_utility {

  void ARFFParser::makeString(const Object& obj, std::string* result) {
    throw std::logic_error("it is impossible to save data in ARFF format");
  }

  DataSet<ObjectPair> ARFFParser::buildPairDataSet(
      const std::vector<Object>& objects,
      const FeatureInfo& info) {
    throw std::logic_error("can't build pairwise dataset for ARFF format");
  }

  DataSet<ObjectList> ARFFParser::buildListDataSet(
      const std::vector<Object>& objects,
      const FeatureInfo& info) {
    throw std::logic_error("can't build listwise dataset for ARFF format");
  }

  Object ARFFParser::parse(const std::string& line,
                           NominalFeatureHandler::Ptr feature_handler) {
    parseNextFeature.init(this);
    parseNextFeature.reset();

    Object obj;
    rule<> number = +digit_p >> !('.' >> *digit_p);
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

    feature_handler->process(features_, &obj.features());
    obj.metaInfo() = meta_features_;

    return obj;
  }

  void ARFFParser::init(std::istream *in) {
    std::string line;

    nominal_feature_info_.feature_type_.clear();
    nominal_feature_info_.feature_values_.clear();

    int feature_id = 0;
    int meta_id = 0;

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
        lexeme_d[+alnum_p][assign_a(attr_name)] >> +space_p >>
        (lexeme_d[+alnum_p][assign_a(attr_type)] |
        ('{' >> (( (quoter >> string_rule[push_back_a(values)] >> quoter) |
                  simple_string[push_back_a(values)]) % ',')
             >> '}') ));
      if (!info.hit)
        throw std::logic_error("can't parse ARFF header");

      boost::to_upper(attr_type);
      nominal_feature_info_.feature_name_[feature_id] = attr_name;
      if (attr_type == "NUMERIC") {
        nominal_feature_info_.feature_type_[feature_id] = NUMERIC;
      } else if (attr_type == "STRING") {
        nominal_feature_info_.feature_type_[feature_id] = META;
      } else if (values.size() != 0) {
        nominal_feature_info_.feature_type_[feature_id] = NOMINAL;
        nominal_feature_info_.feature_values_[feature_id] = values;
      }
      feature_id++;
    }
}

  void ARFFParser::NextFeatureParser::reset() {
    parser_->current_id_ = parser_->current_out_id_ = 0;
    parser_->features_.clear();
    parser_->meta_features_.clear();
  }

  void ARFFParser::NextFeatureParser::operator()(const char* feature_,
                                                 const char* it) const {
    std::string feature(feature_, it);
    int cur_id = parser_->current_id_;
    NominalFeatureType type =
        parser_->nominal_feature_info_.feature_type_[cur_id];
    if (type == META) {
      parser_->meta_features_[
              parser_->nominal_feature_info_.feature_name_[cur_id] ] = feature;
      parser_->current_id_++;
      return;
    }
    parser_->current_id_++;
    if (feature == "?") {
      parser_->current_out_id_++;
      return;
    }
    parser_->features_[parser_->current_out_id_++] = feature;
  }

  void ARFFParser::NextFeatureParser::init(ARFFParser* parser) {
    parser_ = parser;
    info_ = parser->info();
  }
}
}
