// Copyright 2011 Yandex

#ifndef LTR_DATA_UTILITY_IO_UTILITY_H_
#define LTR_DATA_UTILITY_IO_UTILITY_H_

#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <fstream>

#include "ltr/data/data_set.h"
#include "ltr/data/utility/parsers/parser.h"

#include "ltr/scorers/scorer.h"
#include "ltr/utility/numerical.h"

using std::string;
using std::map;
using std::vector;

namespace ltr {
namespace io_utility {
/**
 * Function to load data set from file.
 * @param filename - path to file
 * @param format - data format (svmlite, yandex, arff)
 * @code
 * DataSet<Object> = loadDataSet<Object>("dataset.txt", "svmlite");
 * @endcode
 */
template<class TElement>
DataSet<TElement> loadDataSet(const string& filename,
    const string& format);
/**
 * Function to save data set into file.
 * @param data - data set to save
 * @param filename - path to file
 * @param format - data format (svmlite, yandex, arff) 
 * @code
 * saveDataSet(data, "dataset.txt", "svmlite");
 * @endcode
 */
template<class TElement>
void saveDataSet(const DataSet<TElement>& data,
    const string& filename,
    const string& format);
/**
 * Function to build data set of given objects
 * @param parser - parser, which will build data set
 * @param objects - objects to build data set from
 * @param info - information about features in objects
 * @code
 * buildDataSet<TElement>(parser, objects, handler->featureInfo());
 * @endcode
 */
template<class TElement>
DataSet<TElement> buildDataSet(Parser::Ptr parser,
    const vector<Object>& objects,
    const FeatureInfo& info);
/**
 * Function to save predicted labeles for given data set
 * @param data - data set for prediction
 * @param scorer - scorer for prediction
 * @param filename - path to file
 * @code
 * savePredictions(data, scorer, "predictions.txt");
 * @endcode
 */
template<class TElement>
void savePredictions(const DataSet<TElement>& data,
                     Scorer::Ptr scorer,
                     const string& filename);

/**
 * Function to save predicted labeles for given data set
 * @param data - data set with marked predicted labeles
 * @param filename - path to file
 * @code
 * savePredictions(data, "predictions.txt");
 * @endcode
 */
template<class TElement>
void savePredictions(const DataSet<TElement>& data,
                     const string& filename);
/**
 * Function groups objects in lists, each having same
 * meta-parameter
 * @param objects - objects to group
 * @param group_parameter - meta-parameter name
 * @param result - result lists
 */
void groupByMeta(const vector<Object>& objects,
                 const string& group_parameter,
                 map<string, vector<Object> >* result);
/**
 * Function groups objects in lists, each having same
 * integer meta-parameter
 * @param objects - objects to group
 * @param group_parameter - meta-parameter name
 * @param result - result lists
 */
void groupByIntMeta(const vector<Object>& objects,
                    const string& group_parameter,
                    map<int, vector<Object> >* result);
/**
 * Function groups objects in lists, each having same
 * float meta-parameter
 * @param objects - objects to group
 * @param group_parameter - meta-parameter name
 * @param result - result lists
 */
void groupByFloatMeta(const vector<Object>& objects,
                      const string& group_parameter,
                      map<float, vector<Object> >* result);
};
};

// Template realization
namespace ltr {
namespace io_utility {

template<class TElement>
DataSet<TElement> loadDataSet(const string& filename,
    const string& format) {
  std::ifstream file(filename.c_str());
  if (file.fail())
    throw std::logic_error("File " + filename + " not found");

  Parser::Ptr parser = getParser(format);
  parser->startParsing(&file);

  vector<Object> objects;
  Object current_object;

  while (parser->parseNextObject(&current_object))
    objects.push_back(current_object);

  file.close();
  int feature_cnt = parser->featureInfo().get_feature_count();
  for (size_t i = 0; i < objects.size(); i++)
    for (size_t cn = objects[i].features().size(); cn < feature_cnt; cn++)
      objects[i] << ltr::utility::NaN;
  return buildDataSet<TElement>(parser, objects, parser->featureInfo());
}


template<class TElement>
void saveDataSet(const DataSet<TElement>& data,
                 const string& filename,
                 const string& format) {
  std::ofstream file(filename.c_str());
  if (file.fail())
    throw std::logic_error("can't open " + filename + " for writing");
  Parser::Ptr parser = getParser(format);
  for (size_t i = 0; i < data.size(); i++)
    for (size_t j = 0; j < data[i].size(); j++) {
      string str;
      parser->makeString(data[i][j], &str);
      file << str << std::endl;
    }
  file.close();
}

template<class TElement>
void savePredictions(const DataSet<TElement>& data,
                     Scorer::Ptr scorer,
                     const string& filename) {
  std::ofstream file(filename.c_str());

  if (file.fail()) {
    throw std::logic_error("can't open " + filename + " for writing");
  }

  file.precision(utility::DOUBLE_PRECISION);
  for (size_t i = 0; i < data.size(); i++)
    for (size_t j = 0; j < data[i].size(); j++) {
      file << (*scorer)(data[i][j]) << std::endl;
    }
  file.close();
}

template<class TElement>
void savePredictions(const DataSet<TElement>& data,
                     const string& filename) {
  std::ofstream file(filename.c_str());

  if (file.fail()) {
    throw std::logic_error("can't open " + filename + " for writing");
  }

  file.precision(utility::DOUBLE_PRECISION);
  for (size_t i = 0; i < data.size(); i++)
    for (size_t j = 0; j < data[i].size(); j++) {
      file << boost::lexical_cast<string>(data[i][j].predicted_label())
           << std::endl;
    }
  file.close();
}
};
}
#endif  // LTR_DATA_UTILITY_IO_UTILITY_H_
