// Copyright 2013 Yandex

#ifndef LTR_DATA_UTILITY_PARSERS_PARSE_DSV_H_
#define LTR_DATA_UTILITY_PARSERS_PARSE_DSV_H_

#include <map>
#include <set>
#include <string>
#include <vector>

#include "ltr/data/utility/parsers/parser.h"

using std::set;
using std::string;
using std::vector;

namespace ltr {
namespace io_utility {
/** \brief Parse DSV format
 * (http://en.wikipedia.org/wiki/Delimiter-separated_values)
 * We don't support double quote as screened quote. Please, use backslash for
 * screening.
 */
class DSVParser : public Parser {
 private:
  char delimiter_;
 protected:
  virtual void parseDataInfo(istream& in, // NOLINT
                             FeatureInfo* feature_info,
                             LabelInfo* label_info);
  virtual void parseObject(const string& record,
                           const FeatureInfo& feature_info,
                           const LabelInfo& label_info,
                           Object* object);
 public:
  explicit DSVParser(char delimiter);
  void saveObject(const Object& obj, ostream& stream); // NOLINT
  void saveObjects(const vector<Object>& objects,
                   const FeatureInfo& feature_info,
                   const LabelInfo& label_info,
                   ostream& stream); // NOLINT

  PairwiseDataSet
      buildPairwiseDataSet(const vector<Object>& objects,
                           const FeatureInfo& feature_info,
                           const LabelInfo& label_info);

  ListwiseDataSet
      buildListwiseDataSet(const vector<Object>& objects,
                           const FeatureInfo& feature_info,
                           const LabelInfo& label_info);
};
};
};

#endif  // LTR_DATA_UTILITY_PARSERS_PARSE_DSV_H_
