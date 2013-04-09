// Copyright 2011 Yandex

#ifndef LTR_DATA_UTILITY_PARSERS_PARSE_SVM_H_
#define LTR_DATA_UTILITY_PARSERS_PARSE_SVM_H_

#include <map>
#include <string>
#include <vector>

#include "ltr/data/object.h"
#include "ltr/data/object_pair.h"
#include "ltr/data/object_list.h"
#include "ltr/data/utility/parsers/parser.h"

using std::vector;

namespace ltr {
namespace io_utility {
/** \brief Parse SVM light format (http://svmlight.joachims.org/)
 */
class SVMParser : public Parser {
 protected:
  virtual void parseDataInfo(istream& in, // NOLINT
                             FeatureInfo* feature_info,
                             LabelInfo* label_info);
  virtual void parseObject(const string& record,
                           const FeatureInfo& feature_info,
                           const LabelInfo& label_info,
                           Object* object);

 public:

  virtual void saveObject(const Object& obj, ostream& stream); // NOLINT

  PairwiseDataSet buildPairwiseDataSet(const vector<Object>& objects,
                                       const FeatureInfo& feature_info,
                                       const LabelInfo& label_info);

  ListwiseDataSet buildListwiseDataSet(const vector<Object>& objects,
                                       const FeatureInfo& feature_info,
                                       const LabelInfo& label_info);
};
};
};

#endif  // LTR_DATA_UTILITY_PARSERS_PARSE_SVM_H_
