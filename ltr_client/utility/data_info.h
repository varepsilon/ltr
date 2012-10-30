//Copyright Yandex 2012

#ifndef LTR_CLIENT_UTILITY_DATA_INFO_H_
#define LTR_CLIENT_UTILITY_DATA_INFO_H_

#include <string>

using std::string;
/**
 * Contains the information about the dataset, including
 * name of the dataset, approach in the dataset,
 * the format of the dataset and the file name.
 */
struct DataInfo {
  /**
   * Basic constructor.
   */
  DataInfo() { }
  // C++11 -> we have no initializer, so we MUST write
  // stupid conctructor manually
  /**
   * Another implementation of the basic constructor.
   * @param name_ - string containing the name
   * @param approach_ - string containing approach information
   * @param format_ - string containing format information
   * @param file_name_ - string containing file name information
   */
  DataInfo(const string& name_, const string& approach_,
            const string& format_, const string& file_name_)
    : name(name_)
    , approach(approach_)
    , format(format_)
    , file(file_name_) { }
  string name;
  string approach;
  string format;
  string file;
};
/**
 * Converts DataInfo object into the printable string
 * @param Info - TDataInfo to convert
 * @returns converted string
 */
string ToString(const DataInfo& Info);

#endif //LTR_CLIENT_UTILITY_DATA_INFO_H_

