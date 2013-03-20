// Copyright 2011 Yandex

#include <fstream>

#include "ltr/data/utility/parsers/parse_arff.h"
#include "ltr/data/utility/parsers/parse_svm.h"
#include "ltr/data/utility/parsers/parse_yandex.h"
#include "ltr/data/utility/parsers/parser.h"

#include "ltr/utility/boost/string_utils.h"
#include "ltr/utility/numerical.h"

using std::endl;
using std::getline;
using std::ifstream;
using std::logic_error;
using std::string;

using ltr::utility::to_upper;

namespace ltr {
namespace io_utility {
Parser::Ptr getParser(const string& format) {
  string format_ = format;
  to_upper(&format_);
  if (format_ == "SVMLIGHT")
    return Parser::Ptr(new SVMParser());
  else if (format_ == "YANDEX")
    return Parser::Ptr(new YandexParser());
  else if (format_ == "ARFF")
    return Parser::Ptr(new ARFFParser());
  else
    throw logic_error("unknown format " + format);
}

void Parser::parse(const string& filename,
                   vector<Object>* objects,
                   FeatureInfo* feature_info,
                   LabelInfo* label_info) {
  objects->clear();

  ifstream file(filename.c_str());
  if (file.fail()) {
    throw logic_error("File " + filename + " not found");
  }

  parseDataInfo(file, feature_info, label_info);

  string record;
  while (getline(file, record)) {
    try {
      Object object;
      object.features() = vector<double>(feature_info->feature_count(),
                                         numeric_limits<double>::quiet_NaN());
      parseObject(record, *feature_info, *label_info, &object);
      objects->push_back(object);
    } catch(Parser::bad_line err) {}
  }
}

void Parser::saveObjects(const vector<Object>& objects,
                         const FeatureInfo& feature_info,
                         const LabelInfo& label_info,
                         ostream& stream) { // NOLINT
  for (int object_index = 0;
       object_index < objects.size();
       ++object_index) {
    string str;
    saveObject(objects[object_index], stream);
    stream << str;
  }
}
};
};
