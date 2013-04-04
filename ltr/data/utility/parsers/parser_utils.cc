// Copyright 2013 Yandex

#include <cctype>
#include <stdexcept>

#include "ltr/data/utility/parsers/parser_utils.h"

namespace ltr {
namespace io_utility {

int findFirstUnscreened(const string& str,
                        char symbol,
                        int start_pos,
                        char escape) {
  for (int pos = start_pos; pos < str.size(); ++pos) {
    if (escape != '\0' && str[pos] == escape) {
      ++pos;
    } else if (str[pos] == symbol) {
      return pos;
    }
  }
  return -1;
}

void screenSpecialChars(const string& str,
                        string* result,
                        const string& need_escape,
                        char escape_char) {
  result->clear();
  for (int pos = 0; pos < str.size(); ++pos) {
    if (need_escape.find(str[pos]) != string::npos) {
      *result += escape_char;
    }
    *result += str[pos];
  }
}

void unscreenSpecialChars(const string& str,
                          string* result,
                          char escape_char) {
  result->clear();
  for (int pos = 0; pos < str.size(); ++pos) {
    if (str[pos] == escape_char) {
      ++pos;
    }
    *result += str[pos];
  }
}

void applyQuoting(const string& str,
                  string* result,
                  char left_quote,
                  char right_quote,
                  const string& need_quoting,
                  const string& need_escape,
                  char escape_char) {
  result->clear();
  bool do_quoting = false;
  for (int pos = 0; pos < need_quoting.size(); ++pos) {
    if (str.find(need_quoting[pos]) != string::npos) {
      do_quoting = true;
      break;
    }
  }
  if (do_quoting) {
    screenSpecialChars(str, result, need_escape, escape_char);
    *result = left_quote + *result + right_quote;
  } else {
    *result = str;
  }
}

void escapeSplit(const string& str,
                 vector<string>* result,
                 char delim,
                 char left_quote,
                 char right_quote,
                 char escape_char) {
  result->clear();
  for (int pos = 0; pos < str.size(); ++pos) {
    while (pos < str.size() && str[pos] == ' ') {
      ++pos;
    }
    if (pos == str.size()) {
      break;
    }
    string part;
    if (str[pos] == left_quote) {
      int rpos = findFirstUnscreened(str, right_quote, pos + 1, escape_char);
      if (rpos == -1) {
        throw std::logic_error("escape_split: found left quote but can't found "
            "right quote");
      }
      unscreenSpecialChars(str.substr(pos + 1, rpos - pos - 1),
                           &part, escape_char);
      pos = rpos + 1;
    } else {
      int rpos = pos;
      while (rpos < str.size() && str[rpos] != ' ' && str[rpos] != delim) {
        ++rpos;
      }
      part = str.substr(pos, rpos - pos);
      pos = rpos;
    }
    result->push_back(part);
    while (pos < str.size() && str[pos] == ' ') {
      ++pos;
    }
    if (pos < str.size() && str[pos] != delim) {
      throw std::logic_error("escape_split: garbage between part and "
          "delimiter/EOLN: ");
    }
  }
}
};
};
