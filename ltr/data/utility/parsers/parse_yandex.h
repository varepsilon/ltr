// Copyright 2012 Yandex

#ifndef LTR_DATA_UTILITY_PARSERS_PARSE_YANDEX_H_
#define LTR_DATA_UTILITY_PARSERS_PARSE_YANDEX_H_

#include <map>
#include <vector>
#include <string>
#include <iostream>

#include "ltr/data/object.h"
#include "ltr/data/object_pair.h"
#include "ltr/data/object_list.h"
#include "ltr/data/utility/parsers/parser.h"

using std::vector;

namespace ltr {
namespace io_utility {
/** \brief Parse Yandex Mathematics 2009 format
 * (http://imat2009.yandex.ru/datasets)
 */
class YandexParser : public Parser {
 protected:
  virtual void parseDataInfo(istream& in, // NOLINT
                             FeatureInfo* feature_info,
                             LabelInfo* label_info);
  virtual void parseObject(const string& record,
                           const FeatureInfo& feature_info,
                           const LabelInfo& label_info,
                           Object* object);

 public:

  void saveObject(const Object& obj, ostream& stream); // NOLINT

  PairwiseDataSet buildPairwiseDataSet(const vector<Object>& objects,
                                       const FeatureInfo& feature_info,
                                       const LabelInfo& label_info);

  ListwiseDataSet buildListwiseDataSet(const vector<Object>& objects,
                                       const FeatureInfo& feature_info,
                                       const LabelInfo& label_info);
};
};
};

#endif  // LTR_DATA_UTILITY_PARSERS_PARSE_YANDEX_H_
