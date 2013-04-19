// Copyright 2013 Yandex
#ifndef LTR_DATA_UTILITY_PARSERS_PARSER_UTILS_H_
#define LTR_DATA_UTILITY_PARSERS_PARSER_UTILS_H_

#include <string>
#include <vector>

using std::string;
using std::vector;

namespace ltr {
namespace io_utility {
/**
 * Find first unscreened entry of symbol in given string
 *
 * \param str - string
 * \param symbol - char to find
 * \param start_pos - 0-based index of start position
 * \param escape - escape character ('\0' for no escape character)
 * \return 0-based position of right quote or -1 if it don't exist.
 */
int findFirstUnscreened(const string& str,
                        char symbol,
                        int start_pos = 0,
                        char escape='\\');

/**
 * Encode string so that every "special symbol" is screened.
 *
 * \param str - string to encode
 * \param result - result string
 * \param need_escape - symbols which are needed to be screened
 * \param escape_char - symbol which is used for screening
 */
void screenSpecialChars(const string& str,
                        string* result,
                        const string& need_escape = "'\"\n\\",
                        char escape_char = '\\');

/**
 * Decode string: transform every sequence \<escape_char>\<char> to \<char>.
 *
 * \param str - string to decode
 * \param result - result string
 * \param escape_char - symbol which was used for screening
 */
void unscreenSpecialChars(const string& str,
                          string* result,
                          char escape_char = '\\');

/**
 * Add quotes and screen string if needed
 *
 * \param str - string to encode
 * \param result - result string
 * \param left_quote - symbol of left quote
 * \param right_quote - symbol of right quote
 * \param need_quoting - if string contains such symbol then quoting is needed
 * \param need_escape - symbols which are needed to be screened
 * \param escape_char - symbol which is used for screening
 */
void applyQuoting(const string& str,
                  string* result,
                  char left_quote = '"',
                  char right_quote = '"',
                  const string& need_quoting = "'\" \t\n\\",
                  const string& need_escape = "'\"\n\\",
                  char escape_char = '\\');

/**
 * Split string by delim symbol and unquote quoted parts
 *
 * \param str - string to split
 * \param result - vector of results of split
 * \param delim - delimiter string
 * \param left_quote - symbol of left quote
 * \param right_quote - symbol of right quote
 * \param escape_char - symbol which was used for screening
 */
void escapeSplit(const string& str,
                 vector<string>* result,
                 char delim = ',',
                 char left_quote = '"',
                 char right_quote = '"',
                 char escape_char = '\\');
};
};
#endif  // LTR_DATA_UTILITY_PARSERS_PARSER_UTILS_H_
