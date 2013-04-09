// Copyright 2011 Yandex

#ifndef LTR_DATA_UTILITY_PARSERS_PARSE_ARFF_H_
#define LTR_DATA_UTILITY_PARSERS_PARSE_ARFF_H_

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
/** \brief Parse ARFF format (http://www.cs.waikato.ac.nz/ml/weka/arff.html)
 */
class ARFFParser : public Parser {
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

#endif  // LTR_DATA_UTILITY_PARSERS_PARSE_ARFF_H_
