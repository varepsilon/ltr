// Copyright 2011 Yandex

#ifndef LTR_INTERFACES_REPORTER_H_
#define LTR_INTERFACES_REPORTER_H_

#include <string>

using std::string;

namespace ltr {
  class Reporter {
  public:
    string report() const {
        return report_;
    }

    Reporter() {}
    explicit Reporter(const string& input)
        : report_(input) {}

    void clearReport() {
        report_.clear();
    }

    void appendReport(const string& input) {
        report_.append(input);
    }

    void setReport(const string& input) {
        report_ = input;
    }

  private:
    string report_;
  };
};

#endif  // LTR_INTERFACES_REPORTER_H_
