// Copyright 2013 Yandex

#include "ltr/utility/boost/lexical_cast_config.hpp"

#include <sstream>

using std::stringstream;
using std::fixed;

namespace ltr {
namespace utility {
LexicalCastConfig& LexicalCastConfig::getInstance() {
  if (instance_ == NULL) {
    instance_ = new LexicalCastConfig;
  }
  return *instance_;
}
LexicalCastConfig* LexicalCastConfig::instance_ = NULL;

void LexicalCastConfig::restoreDefaults() {
  precision_ = -1;
  fixed_ = false;
}
void LexicalCastConfig::setPrecision(int precision) {
  precision_ = precision;
}
void LexicalCastConfig::setFixed(bool fixed) {
  fixed_ = fixed;
}
void LexicalCastConfig::configure(stringstream* stream) const {
  if (fixed_) {
    (*stream) << fixed;
  }
  if (precision_ != -1) {
    stream->precision(precision_);
  }
}

LexicalCastConfig::LexicalCastConfig() {
  restoreDefaults();
}
}
}
