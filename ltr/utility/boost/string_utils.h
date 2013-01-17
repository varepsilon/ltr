// Copyright 2011 Yandex

#ifndef LTR_UTILITY_BOOST_STRING_UTILS_H_
#define LTR_UTILITY_BOOST_STRING_UTILS_H_

#include <locale>
#include <string>
#include <vector>
#include <exception>

using std::string;
using std::vector;
using std::locale;

namespace ltr {
namespace utility {
/**
 * trim_copy returns a copy of the string with the leading and trailing
 * separators removed.
 * trim removes leading and trailing separators from given input string.
 * trim(vector<string>) removes leading and trailing separators from each
 * string. Empty strings remain.
 * separator is a string specifying the set of characters to be removed.
 */
string trim_copy(const string& input, const string& separator = " \r\n\t");
void trim_copy(const vector<string>& input,
               vector<string>* result,
               const string& separator = " \r\n\t");
vector<string> trim_copy(const vector<string>& input,
                         const string& separator = " \r\n\t");

void trim(string* input, const string& separator = " \r\n\t");
void trim(vector<string>* input, const string& separator = " \r\n\t");
/**
 * Return a vector of the words in the string, using separator as the delimiter
 * string. Input - input string or vector<string> to be splitted. 
 * Separator - separator to split input data. If separator not given, input data
 * will be splitted by spaces, consecutive space characters will be
 * consedered as one separator and leading and trailing spaces will be removed.
 * If maxsplit is given, at most maxsplit splits are done (thus, the list will 
 * have at most maxsplit+1 elements). If maxsplit is not specified or -1, then 
 * there is no limit on the number of splits (all possible splits are made).
 * List of words will be placed in result, which will be cleared in split and
 * won't in split_append, or in return value. If result parameter and return 
 * value are not specified, result will be placed in vector<string> data, same
 * as input list of string.
 */
void split(const string& input,
           const string& separator,
           vector<string>* result,
           int maxsplit = -1);
void split_append(const string& input,
                  const string& separator,
                  vector<string>* result,
                  int maxsplit = -1);
void split(const string& input,
           vector<string>* result,
           int maxsplit = -1,
           const locale& locale = std::locale());
void split_append(const string& input,
                  vector<string>* result,
                  int maxsplit = -1,
                  const locale& locale = std::locale());
void split(const vector<string>& input,
           const string& separator,
           vector<string>* result,
           int maxsplit = -1);
void split_append(const vector<string>& input,
                  const string& separator,
                  vector<string>* result,
                  int maxsplit);
void split(const vector<string>& input,
           vector<string>* result,
           int maxsplit = -1);
void split_append(const vector<string>& input,
                  vector<string>* result,
                  int maxsplit = -1);
void split(vector<string>* data,
           const string& separator,
           int maxsplit = -1);
void split(vector<string>* data,
           int maxsplit = -1);
vector<string> split(const string& input,
                     const string& separator,
                     int maxsplit = -1);
vector<string> split(const string& input,
                     int maxsplit = -1);
vector<string> split(const vector<string>& input,
                     const string& separator,
                     int maxsplit = -1);
vector<string> split(const vector<string>& input,
                     int maxsplit = -1);

/**
 * Converts all charachters in string to upper case using given locale
 */
void to_upper(string* input, const locale& loc = std::locale());
/**
 * Converts all charachters in string to lower case using given locale
 */
void to_lower(string* input, const locale& loc = std::locale());
/**
 * Сonsequentially erases all occurences of string search
 */
void erase_all(string* input, const string& search);
};
};
#endif  // LTR_UTILITY_BOOST_STRING_UTILS_H_
