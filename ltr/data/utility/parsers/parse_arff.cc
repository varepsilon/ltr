// Copyright 2011 Yandex

#include <sstream>
#include <stdexcept>

#include "ltr/data/utility/parsers/parse_arff.h"
#include "ltr/data/utility/parsers/parser_utils.h"
#include "ltr/utility/boost/lexical_cast.h"
#include "ltr/utility/boost/string_utils.h"
#include "ltr/utility/numerical.h"

using std::endl;
using std::getline;
using std::logic_error;
using std::map;
using std::stringstream;

using ltr::io_utility::escapeSplit;
using ltr::utility::lexical_cast;
using ltr::utility::NaN;
using ltr::utility::split;
using ltr::utility::to_upper;
using ltr::utility::trim;
using ltr::utility::trim_copy;

namespace ltr {
namespace io_utility {
void ARFFParser::parseDataInfo(istream& in, // NOLINT
                               FeatureInfo* feature_info,
                               LabelInfo* label_info) {
  string line;
  bool class_attribute_found = false;

  while (getline(in, line)) {
    // skip lines without useful information
    if (line.size() == 0 || line[0] != '@') {
      continue;
    }

    // drop first '@' symbol
    line = line.substr(1, line.size() - 1);
    trim(&line);
    string line_type;
    vector<string> splitted;

    // drop comments
    split(line, "%", &splitted, 1);
    if (splitted.size() >= 1) {
      line = splitted[0];
    }
    trim(&line);

    // extract type of record
    split(line, &splitted, 2);
    if (splitted.size() >= 1) {
      line_type = splitted[0];
    }

    to_upper(&line_type);
    // if data section begins -- stop reading metadata
    // we don't use name of relation, so skip this line
    // \TODO(freopen) we can use it as alias of DataSet
    // if type is ATTRIBUTE -- continue parsing
    if (line_type == "DATA") {
      break;
    } else if (line_type == "RELATION") {
      continue;
    } else if (line_type != "ATTRIBUTE") {
      throw logic_error("can't parse ARFF header: unknown parameter");
    }

    string attr_name;
    string attr_type;
    vector<string> values;

    // extract name and type of attribute
    split(line, &splitted);
    if (splitted.size() != 3) {
      throw logic_error("can't parse ARFF header: bad @ATTRIBUTE field");
    }
    attr_name = splitted[1];
    attr_type = splitted[2];

    // if attribute is nominal -- parse possible values
    if (attr_type[0] == '{') {
      if (attr_type[attr_type.size() - 1] != '}') {
        throw logic_error("can't parse ARFF header: "
            "bad nominal attribute");
      }
      attr_type = attr_type.substr(1, attr_type.size() - 2);
      escapeSplit(attr_type, &values, ',', '\'', '\'');
    }

    // fill OneFeatureInfo using extracted data
    to_upper(&attr_type);
    OneFeatureInfo feature;
    feature.name_ = attr_name;
    if (attr_type == "NUMERIC" || attr_type == "REAL") {
      feature.type_ = NUMERIC;
    } else if (attr_type == "STRING") {
      throw logic_error("can't parse ARFF header: strings are not allowed");
    } else if (values.size() != 0) {
      feature = OneFeatureInfo(values);
    } else {
      throw logic_error("can't parse ARFF header: unknown attribute type");
    }
    if (attr_name == "class") {
      *label_info = feature;
      class_attribute_found = true;
    } else {
      if (class_attribute_found) {
        // \TODO(freopen) find way to avoid this restriction
        throw logic_error("class attribute must be last");
      }
      feature_info->addFeature(feature);
    }
  }
}

void ARFFParser::parseObject(const string& record,
                             const FeatureInfo& feature_info,
                             const LabelInfo& label_info,
                             Object* object) {
  // drop empty strings and comments
  if (record.size() == 0 || record[0] == '%') {
    throw Parser::bad_line();
  }

  // split string into values list
  vector<string> values;
  escapeSplit(trim_copy(record), &values, ',', '\'', '\'');
  trim(&values);

  // parse every value
  for (int value_index = 0; value_index < values.size(); ++value_index) {
    string feature = values[value_index];
    OneFeatureInfo info;
    if (value_index < feature_info.feature_count()) {
      info = feature_info[value_index];
    } else if (value_index == feature_info.feature_count()) {
      info = label_info;
    } else {
      throw logic_error("can't parse ARFF object: too much fields");
    }

    double value;
    if (feature == "?") {
      value = NaN;
    } else if (info == NOMINAL) {
      value = hash(feature);
    } else {
      value = lexical_cast<double>(feature);
    }

    // push extracted value into object
    if (value_index < feature_info.feature_count()) {
      object->at(value_index) = value;
    } else {
      object->set_actual_label(value);
    }
  }
}

void ARFFParser::saveObject(const Object& object, ostream& stream) { // NOLINT
  throw logic_error("Can't make string from single object");
}

void ARFFParser::saveObjects(const vector<Object>& objects,
                             const FeatureInfo& feature_info,
                             const LabelInfo& label_info,
                             ostream& stream) { // NOLINT
  stream.precision(utility::DOUBLE_PRECISION);
  stream << "@RELATION 'LTR dataset'" << endl << endl;
  for (int feature_index = 0;
      feature_index < feature_info.feature_count();
      ++feature_index) {
    if (feature_info.getFeatureType(feature_index) == ltr::NOMINAL) {
      stream << "@ATTRIBUTE attr" << feature_index << " {";
      vector<string> nominals;
      feature_info.getNominalFeatureValues(feature_index, &nominals);
      for (int value_index = 0; value_index < nominals.size(); ++value_index) {
        if (value_index != 0)
          stream << ",";
        stream << nominals[value_index];
      }
      stream << "}" << endl;
    } else {
      stream << "@ATTRIBUTE attr" << feature_index << " NUMERIC" << endl;
    }
  }
  stream << "@ATTRIBUTE class NUMERIC" << endl;
  stream << endl << "@DATA" << endl;
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
      stream << ',';
    }
    stream << object.actual_label();
    if (object_index + 1 < objects.size())
      stream << endl;
  }
}


PairwiseDataSet
    ARFFParser::buildPairwiseDataSet(const vector<Object>& objects,
                                     const FeatureInfo& feature_info,
                                     const LabelInfo& label_info) {
  throw logic_error("can't build pairwise dataset for ARFF format");
}

ListwiseDataSet
    ARFFParser::buildListwiseDataSet(const vector<Object>& objects,
                                     const FeatureInfo& feature_info,
                                     const LabelInfo& label_info) {
  throw logic_error("can't build listwise dataset for ARFF format");
}
}
}
