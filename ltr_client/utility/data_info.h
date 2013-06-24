// Copyright Yandex 2012

#ifndef LTR_CLIENT_UTILITY_DATA_INFO_H_
#define LTR_CLIENT_UTILITY_DATA_INFO_H_

#include <string>

#include "ltr/interfaces/printable.h"
#include "ltr/utility/macros.h"

using std::string;

using ltr::Printable;

/**
 * \brief Contains the information about the dataset as text.
 */
struct DataInfo: public Printable {
  ALLOW_SHARED_PTR_ONLY_CREATION(DataInfo);
 public:
  string toString() const {
    return "DataInfo(name=" + name +
        ", approach=" + approach +
        ", format=" + format +
        ", file_name=" + file +
        ")";
  }
  /**
   * Basic constructor
   */
  DataInfo() { }
  /**
   * Basic constructor.
   *
   * \param name_ - string containing the name
   * \param approach_ - string containing approach information
   * \param format_ - string containing format information
   * \param file_name_ - string containing file name information
   */
  DataInfo(const string& name_, const string& approach_,
            const string& format_, const string& file_name_)
    : name(name_)
    , approach(approach_)
    , format(format_)
    , file(file_name_) { }
  /**
   * Name of the dataset.
   */
  string name;
  /**
   * Approach of the dataset.
   */
  string approach;
  /**
   * Format of the dataset.
   */
  string format;
  /**
   * File that contains dataset.
   */
  string file;
};

#endif  // LTR_CLIENT_UTILITY_DATA_INFO_H_

