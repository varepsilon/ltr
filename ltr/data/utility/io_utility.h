// Copyright 2011 Yandex

#ifndef LTR_DATA_UTILITY_IO_UTILITY_H_
#define LTR_DATA_UTILITY_IO_UTILITY_H_

#include <fstream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#include "ltr/utility/boost/path.h"

#include "ltr/data/data_set.h"
#include "ltr/data/utility/parsers/parser.h"
#include "ltr/scorers/scorer.h"
#include "ltr/utility/numerical.h"
#include "ltr/utility/boost/lexical_cast.h"

using std::ifstream;
using std::map;
using std::ofstream;
using std::string;
using std::vector;

using ltr::utility::DOUBLE_PRECISION;
using ltr::io_utility::Parser;

namespace ltr {
namespace io_utility {
/**
 * Function to load data set from file.
 *
 * \param filename is a path to file
 * \param format is data format (SVMLIGHT, yandex, arff)
 * \code
 * DataSet<Object> = loadDataSet<Object>("dataset.txt", "SVMLIGHT");
 * \endcode
 */
template<class TElement>
DataSet<TElement> loadDataSet(const string& filename, const string& format);
/**
 * Function to save data set into file.
 *
 * \param data is data set to save
 * \param filename is path to file
 * \param format is data format (SVMLIGHT, yandex, arff)
 * \code
 * saveDataSet(data, "dataset.txt", "SVMLIGHT");
 * \endcode
 */
template<class TElement>
void saveDataSet(const DataSet<TElement>& data,
                 const string& filename,
                 const string& format);
/**
 * Function to build data set of given objects
 *
 * \param parser is a parser, which will build data set
 * \param objects are objects to build data set from
 * \param feature_info is information about features in objects
 * \param label_info is information about labels in objects
 * \code
 * buildDataSet<TElement>(
 *     parser,
 *     objects,
 *     handler->featureInfo(),
 *     handler->labelInfo());
 * \endcode
 */
template<class TElement>
DataSet<TElement> buildDataSet(Parser::Ptr parser,
                               const vector<Object>& objects,
                               const FeatureInfo& feature_info,
                               const LabelInfo& label_info);
/**
 * Function to save label labeles for given data set
 *
 * \param data is data set for prediction
 * \param scorer is scorer for prediction
 * \param filename is path to file
 * \code
 * savePredictions(data, scorer, "predictions.txt");
 * \endcode
 */
template<class TElement>
void savePredictions(const DataSet<TElement>& data,
                     Scorer::Ptr scorer,
                     const string& filename);

/**
 * Function to save predicted labeles for given data set
 *
 * \param data is data set with marked predicted labeles
 * \param filename is path to file
 * \code
 * savePredictions(data, "predictions.txt");
 * \endcode
 */
template<class TElement>
void savePredictions(const DataSet<TElement>& data,
                     const string& filename);
/**
 * Function groups objects in lists, each having same meta-parameter
 *
 * \param objects are objects to group
 * \param group_parameter is meta-parameter name
 * \param result is result lists
 */
void groupByMeta(const vector<Object>& objects,
                 const string& group_parameter,
                 map<string, vector<Object> >* result);
/**
 * Function groups objects in lists, each having same integer meta-parameter
 *
 * \param objects are objects to group
 * \param group_parameter is meta-parameter name
 * \param result is result lists
 */
void groupByIntMeta(const vector<Object>& objects,
                    const string& group_parameter,
                    map<int, vector<Object> >* result);
/**
 * Function groups objects in lists, each having same float meta-parameter
 *
 * \param objects are objects to group
 * \param group_parameter are meta-parameter name
 * \param result are result lists
 */
void groupByFloatMeta(const vector<Object>& objects,
                      const string& group_parameter,
                      map<float, vector<Object> >* result);
/**
 *\ Function groups objects in lists, each having same double meta-parameter
 *
 * \param objects are objects to group
 * \param group_parameter is meta-parameter name
 * \param result is result lists
 */
void groupByDoubleMeta(const vector<Object>& objects,
                      const string& group_parameter,
                      map<double, vector<Object> >* result);
};
};

// Template realization
namespace ltr {
namespace io_utility {

template<class TElement>
DataSet<TElement> loadDataSet(const string& filename,
                              const string& format) {
  Parser::Ptr parser = getParser(format);
  vector<Object> objects;
  FeatureInfo feature_info;
  LabelInfo label_info;
  parser->parse(filename, &objects, &feature_info, &label_info);
  DataSet<TElement> data_set = buildDataSet<TElement>(parser, objects,
                                                      feature_info,
                                                      label_info);
  data_set.set_alias(filename);
  return data_set;
}


template<class TElement>
void saveDataSet(const DataSet<TElement>& data,
                 const string& filename,
                 const string& format) {
  ofstream file(filename.c_str());
  if (file.fail()) {
    throw std::logic_error("can't open " + filename + " for writing");
  }
  Parser::Ptr parser = getParser(format);
  string str;
  vector<Object> objects;
  for (int element_index = 0;
       element_index < (int)data.size();
       ++element_index) {
    PerObjectAccessor<const TElement> per_object_accessor(&data[element_index]);
    for (int object_index = 0;
         object_index < per_object_accessor.object_count();
         ++object_index) {
      objects.push_back(per_object_accessor.object(object_index));
    }
  }
  parser->saveObjects(objects, data.feature_info(), data.label_info(), file);
  file << std::endl;
  file.close();
}

template<class TElement>
void savePredictions(const DataSet<TElement>& data,
                     Scorer::Ptr scorer,
                     const string& filename) {
  ofstream file(filename.c_str());

  if (file.fail()) {
    throw std::logic_error("can't open " + filename + " for writing");
  }

  file.precision(DOUBLE_PRECISION);
  for (int element_index = 0;
       element_index < (int)data.size();
       ++element_index) {
    PerObjectAccessor<const TElement> per_object_accessor(&data[element_index]);
    for (int object_index = 0;
         object_index < per_object_accessor.object_count();
         ++object_index) {
      file << (*scorer)(per_object_accessor.object(object_index)) << std::endl;
    }
  }
  file.close();
}

template<class TElement>
void savePredictions(const DataSet<TElement>& data,
                     const string& filename) {
  ofstream file(filename.c_str());

  if (file.fail()) {
    throw std::logic_error("can't open " + filename + " for writing");
  }

  file.precision(utility::DOUBLE_PRECISION);
  for (int element_index = 0;
       element_index < (int)data.size();
       ++element_index) {
    PerObjectAccessor<const TElement> per_object_accessor(&data[element_index]);
    for (int object_index = 0;
         object_index < per_object_accessor.object_count();
         ++object_index) {
      double label = per_object_accessor.object(object_index).predicted_label();
      file << label << std::endl;
    }
  }
  file.close();
}
};
};
#endif  // LTR_DATA_UTILITY_IO_UTILITY_H_
