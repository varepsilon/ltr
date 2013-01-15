// Copyright 2011 Yandex

#include <stdexcept>

#include "ltr/utility/boost/string_utils.h"

namespace ltr {
namespace utility {
string trim_copy(const string& input, const string& separator) {
  int first_not_of = input.find_first_not_of(separator);
  if (first_not_of == string::npos)
    return "";
  else
    return input.substr(first_not_of,
                        input.find_last_not_of(separator) - first_not_of + 1);
}

void trim(string* input, const string& separator) {
  int first_not_of = input->find_first_not_of(separator);
  int last_not_of = input->find_last_not_of(separator);
  if (first_not_of == string::npos || last_not_of == string::npos) {
    input->resize(0);
    return;
  }
  int new_length = last_not_of - first_not_of + 1;
  for (int i = 0; i < new_length; ++i)
    (*input)[i] = (*input)[i + first_not_of];
  input->resize(new_length);
}

void trim_copy(const vector<string>& input,
               vector<string>* result,
               const string& separator) {
  result->clear();
  result->resize(input.size());
  for (int i = 0; i < input.size(); ++i)
    (*result)[i] = trim_copy(input[i], separator);
}

void trim(vector<string>* input, const string& separator) {
  for (int i = 0; i < input->size(); ++i)
    trim(&((*input)[i]), separator);
}

vector<string> trim_copy(const vector<string>& input, const string& separator) {
  vector<string> result;
  trim_copy(input, &result, separator);
  return result;
}

int find_first_space(const string& input, int start_pos, const locale& locale) {
  for (int i = start_pos; i < static_cast<int>(input.size()); ++i)
    if (isspace(input[i], locale))
      return i;
  return input.size();
}
int find_first_not_space(const string& input,
                         int start_pos,
                         const locale& locale) {
  for (int i = start_pos; i < static_cast<int>(input.size()); ++i)
    if (!isspace(input[i], locale))
      return i;
  return input.size();
}

void split_append(const string& input,
                  const string& separator,
                  vector<string>* result,
                  int maxsplit) {
  if (separator.empty())
    throw std::invalid_argument("split: separator cannot be empty");
  int str_pos = 0;
  int num_splited = 0;
  while (maxsplit == -1 || num_splited < maxsplit) {
    int sep_pos = input.find(separator, str_pos);
    if (sep_pos == string::npos)
      break;
    result->push_back(input.substr(str_pos, sep_pos - str_pos));
    ++num_splited;
    str_pos = sep_pos + separator.size();
  }
  result->push_back(input.substr(str_pos, input.size() - str_pos));
}

void split(const string& input,
           const string& separator,
           vector<string>* result,
           int maxsplit) {
  result->clear();
  split_append(input, separator, result, maxsplit);
}

void split_append(const string& input,
                  vector<string>* result,
                  int maxsplit,
                  const locale& locale) {
  int str_pos = 0;
  int num_splited = 0;
  while (maxsplit == -1 || num_splited < maxsplit) {
    int start_pos = find_first_not_space(input, str_pos, locale);
    if (start_pos == input.size())
      break;
    int end_pos = find_first_space(input, start_pos, locale);
    result->push_back(input.substr(start_pos, end_pos - start_pos));
    ++num_splited;
    str_pos = end_pos;
  }
  int start_pos = find_first_not_space(input, str_pos, locale);
  if (start_pos != input.size())
    result->push_back(input.substr(start_pos, input.size() - start_pos));
}

void split(const string& input,
           vector<string>* result,
           int maxsplit,
           const locale& locale) {
  result->clear();
  split_append(input, result, maxsplit, locale);
}

void split_append(const vector<string>& input,
                  const string& separator,
                  vector<string>* result,
                  int maxsplit) {
  if (separator.empty())
    throw std::invalid_argument("split: separator cannot be empty");
  int n_of_splits = 0;
  int new_maxsplit = maxsplit;
  for (int i = 0; i < static_cast<int>(input.size()); ++i) {
    n_of_splits = result->size() - i;
    new_maxsplit = std::min(maxsplit, std::max(maxsplit-n_of_splits, 0));
    split_append(input[i], separator, result, new_maxsplit);
  }
}

void split(const vector<string>& input,
           const string& separator,
           vector<string>* result,
           int maxsplit) {
  result->clear();
  split_append(input, separator, result, maxsplit);
}

void split_append(const vector<string>& input,
                  vector<string>* result,
                  int maxsplit) {
  result-> clear();
  int n_of_splits = 0;
  int new_maxsplit = maxsplit;
  for (int i = 0; i < static_cast<int>(input.size()); ++i) {
    n_of_splits = result->size() - i;
    new_maxsplit = std::min(maxsplit, std::max(maxsplit-n_of_splits, 0));
    split_append(input[i], result, new_maxsplit);
  }
}

void split(const vector<string>& input,
           vector<string>* result,
           int maxsplit) {
  result->clear();
  split_append(input, result, maxsplit);
}

void split(vector<string>* data,
           const string& separator,
           int maxsplit) {
  vector<string> data_copy = *data;
  split(data_copy, separator, data, maxsplit);
}

void split(vector<string>* data,
           int maxsplit) {
  vector<string> data_copy = *data;
  split(data_copy, data, maxsplit);
}

vector<string> split(const string& input,
                     const string& separator,
                     int maxsplit) {
  vector<string> result;
  split(input, separator, &result, maxsplit);
  return result;
}

vector<string> split(const string& input, int maxsplit) {
  vector<string> result;
  split(input, &result, maxsplit);
  return result;
}

vector<string> split(const vector<string>& input,
                     const string& separator,
                     int maxsplit) {
  vector<string> result;
  split(input, separator, &result, maxsplit);
  return result;
}

vector<string> split(const vector<string>& input, int maxsplit) {
  vector<string> result;
  split(input, &result, maxsplit);
  return result;
}

void to_upper(string* input, const std::locale& loc) {
  for (int it = 0; it < input->size(); ++it)
    (*input)[it] = std::toupper((*input)[it], loc);
}

void to_lower(string* input, const std::locale& loc) {
  for (int it = 0; it < input->size(); ++it)
    (*input)[it] = std::tolower((*input)[it], loc);
}

void erase_all(string* input, const string& search) {
  if (search.empty())
    return;
  int pos = input->find(search);
  while (pos != string::npos) {
    input->erase(pos, search.size());
    pos = input->find(search, pos);
  }
}
};
};
