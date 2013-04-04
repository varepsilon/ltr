// Copyright 2011 Yandex

#include <sstream>
#include <stdexcept>

#include <iostream>
#include "ltr/data/utility/parsers/parse_dsv.h"
#include "ltr/data/utility/parsers/parser_utils.h"
#include "ltr/utility/boost/lexical_cast.h"
#include "ltr/utility/boost/string_utils.h"
#include "ltr/utility/numerical.h"

using std::endl;
using std::getline;
using std::logic_error;
using std::map;
using std::streampos;
using std::stringstream;

using ltr::io_utility::applyQuoting;
using ltr::io_utility::escapeSplit;
using ltr::NominalFeatureValues;
using ltr::utility::bad_lexical_cast;
using ltr::utility::lexical_cast;
using ltr::utility::NaN;
using ltr::utility::split;
using ltr::utility::to_upper;
using ltr::utility::trim;
using ltr::utility::trim_copy;

namespace ltr {
namespace io_utility {
DSVParser::DSVParser(char delimiter)
    : delimiter_(delimiter) {}

void DSVParser::parseDataInfo(istream& in, // NOLINT
                              FeatureInfo* feature_info,
                              LabelInfo* label_info) {
  string first_line;
  do {
    getline(in, first_line);
    trim(&first_line);
  } while (first_line.size() == 0);
  vector<string> names;
  escapeSplit(first_line, &names, delimiter_);
  int feature_count = names.size();
  vector<FeatureType> feature_types(feature_count, NUMERIC);

  streampos first_object_pos = in.tellg();
  string line;
  while (getline(in, line)) {
    trim(&line);
    if (line.size() == 0) {
      continue;
    }
    vector<string> values;
    escapeSplit(line, &values, delimiter_);
    if (values.size() != feature_count) {
      throw logic_error("can't parse DSV: incorrect number of values");
    }
    for (int value_index = 0; value_index < feature_count; ++value_index) {
      if (feature_types[value_index] == NUMERIC) {
        try {
          lexical_cast<double>(values[value_index]);
        } catch(bad_lexical_cast err) {
          feature_types[value_index] = NOMINAL;
        }
      }
    }
  }

  feature_info->clear();
  for (int feature_index = 0; feature_index < feature_count; ++feature_index) {
    feature_info->addFeature(feature_types[feature_index],
                             NominalFeatureValues(),
                             names[feature_index]);
  }

  in.clear();
  in.seekg(first_object_pos);
  while (getline(in, line)) {
    trim(&line);
    if (line.size() == 0) {
      continue;
    }
    vector<string> values;
    escapeSplit(line, &values, delimiter_);
    for (int value_index = 0; value_index < feature_count; ++value_index) {
      if (feature_types[value_index] == NOMINAL) {
        feature_info->addNominalFeatureValue(value_index, values[value_index]);
      }
    }
  }

  in.clear();
  in.seekg(first_object_pos);
}

void DSVParser::parseObject(const string& record,
                            const FeatureInfo& feature_info,
                            const LabelInfo& label_info,
                            Object* object) {
  // drop empty strings and comments
  if (record.size() == 0) {
    throw Parser::bad_line();
  }

  // split string into values list
  vector<string> values;
  escapeSplit(trim_copy(record), &values, delimiter_);
  trim(&values);

  // parse every value
  for (int value_index = 0; value_index < values.size(); ++value_index) {
    FeatureType feature_type = feature_info.getFeatureType(value_index);
    double value;
    if (values[value_index] == "?") {
      value = NaN;
    } else if (feature_type == NOMINAL) {
      value = hash(values[value_index]);
    } else {
      value = lexical_cast<double>(values[value_index]);
    }

    // push extracted value into object
    object->at(value_index) = value;
  }
}

void DSVParser::saveObject(const Object& object, ostream& stream) { // NOLINT
  throw logic_error("Can't make string from single object");
}

void DSVParser::saveObjects(const vector<Object>& objects,
                            const FeatureInfo& feature_info,
                            const LabelInfo& label_info,
                            ostream& stream) { // NOLINT
  stream.precision(utility::DOUBLE_PRECISION);
  for (int feature_index = 0;
       feature_index < feature_info.feature_count();
       ++feature_index) {
    string feature_name;
    applyQuoting(feature_info.getFeatureName(feature_index), &feature_name);
    if (feature_name.size() == 0) {
      feature_name = "attribute" + lexical_cast<string>(feature_index + 1);
    }
    stream << feature_name;
    if (feature_index + 1 < feature_info.feature_count()) {
      stream << delimiter_;
    }
  }
  stream << endl;

  for (int object_index = 0; object_index < objects.size(); ++object_index) {
    const Object& object = objects[object_index];
    for (int feature_index = 0;
        feature_index < object.feature_count();
        ++feature_index) {
      switch (feature_info.getFeatureType(feature_index)) {
        case NUMERIC:
        case BOOLEAN:
          stream << object[feature_index];
          break;
        case NOMINAL:
          stream << feature_info.getNominalFeatureValue(
              feature_index, object[feature_index]);
          break;
      }
      if (feature_index + 1 < feature_info.feature_count()) {
        stream << delimiter_;
      }
    }
    stream << endl;
  }
}


PairwiseDataSet
    DSVParser::buildPairwiseDataSet(const vector<Object>& objects,
                                    const FeatureInfo& feature_info,
                                    const LabelInfo& label_info) {
  throw logic_error("can't build pairwise dataset for DSV format");
}

ListwiseDataSet
    DSVParser::buildListwiseDataSet(const vector<Object>& objects,
                                    const FeatureInfo& feature_info,
                                    const LabelInfo& label_info) {
  throw logic_error("can't build listwise dataset for DSV format");
}
}
}
