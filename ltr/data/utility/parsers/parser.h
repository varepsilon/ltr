// Copyright 2011 Yandex

#ifndef LTR_DATA_UTILITY_PARSERS_PARSER_H_
#define LTR_DATA_UTILITY_PARSERS_PARSER_H_

#include <istream>
#include <ostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "ltr/data/data_set.h"
#include "ltr/data/object.h"
#include "ltr/utility/murmur_hash.h"

using std::istream;
using std::logic_error;
using std::ostream;
using std::string;
using std::stringstream;
using std::vector;

namespace ltr {
namespace io_utility {
/**
 * \brief This class allows to parse strings into Objects
 * and serialize Objects into strings.
 */
class Parser {
 public:
  class bad_line : public logic_error {
   public:
    bad_line() : logic_error("") {}
  };

  typedef ltr::utility::shared_ptr<Parser> Ptr;
  /**
   * Parse file into PointwiseDataSet
   *
   * \param filename is a name of file with data set
   * \param objects is a vector of extracted objects
   * \param feature_info is a feature information of extracted data set
   * \param label_info is label information of extracted data set
   */
  void parse(const string& filename,
             vector<Object>* objects,
             FeatureInfo* feature_info,
             LabelInfo* label_info);
  /**
   * Function serializes object into string.
   *
   * \param object is object to serialize
   * \param result is ostream to output string representation
   * \code
   * parser->saveObject(object, cout);
   * \endcode
   */
  virtual void saveObject(const Object& object, ostream& result) = 0; // NOLINT
  /**
   * Functions serializes dataset into string.
   *
   * \param objects - vector of dataset's objects
   * \param feature_info - information about feature types
   * \param label_info - information about label type
   * \param result - ostream to output string representation
   * \code
   * parser->saveObjects(object_list, feature_info, label_info, cout);
   * \endcode
   */
  virtual void saveObjects(const vector<Object>& objects,
                           const FeatureInfo& feature_info,
                           const LabelInfo& label_info,
                           ostream& result); // NOLINT
  /**
   * Function to create Pairwise data set of given objects
   *
   * \param objects - objects to create data set from
   * \param feature_info - information about features in the object
   * \param label_info - information about label values
   */
  virtual PairwiseDataSet buildPairwiseDataSet(const vector<Object>& objects,
                                               const FeatureInfo& feature_info,
                                               const LabelInfo& label_info) = 0;
  /**
   * Function to create Listwise data set of given objects
   *
   * \param objects - objects to create data set from
   * \param feature_info - information about features in the object
   * \param label_info - information about label values
   */
  virtual ListwiseDataSet buildListwiseDataSet(const vector<Object>& objects,
                                               const FeatureInfo& feature_info,
                                               const LabelInfo& label_info) = 0;

  virtual ~Parser() {}

 protected:
  /**
   * Realization of parsing data information for format. Function must set 'in'
   * onto first symbol of first object.
   *
   * \param in - input stream with file
   * \param feature_info - parsed information about features
   * \param label_info - parsed information about label values
   */
  virtual void parseDataInfo(istream& in, // NOLINT
                             FeatureInfo* feature_info,
                             LabelInfo* label_info) = 0;
  /**
   * Realization of parsing object by its string representation. Every line
   * from file starting from position after parsing data information will be
   * casted into object by this function
   *
   * \param record - string which contains object to parse
   * \param feature_info - information about features
   * \param label_info - information about label values
   * \param object - result of parsing
   */
  virtual void parseObject(const string &record,
                           const FeatureInfo& feature_info,
                           const LabelInfo& label_info,
                           Object* object) = 0;
};

/**
 * Returns the parser for given format.
 *
 * \param format - format to create parser for.
 * \code
 * Parser::Ptr yandex_parser = getParser("yandex");
 * \endcode
 */
Parser::Ptr getParser(const string& format);
};
};
#endif  // LTR_DATA_UTILITY_PARSERS_PARSER_H_
