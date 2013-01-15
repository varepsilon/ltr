// Copyright 2012 Yandex

#ifndef LTR_UTILITY_PATH_H_
#define LTR_UTILITY_PATH_H_

#include <string>

using std::string;

namespace ltr {
namespace utility {

string FixPathSeparators(const string& path);

string AppendTrailingPathSeparator(const string& path);

string GetFileName(const string& path);

bool IsAbsoluteFileName(const string& text);
}
}


#endif
