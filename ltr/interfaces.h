// Copyright 2011 Yandex

#ifndef LTR_INTERFACES_H_
#define LTR_INTERFACES_H_

#include <string>
#include <vector>

#include "ltr/data/object.h"
#include "ltr/data/object_pair.h"
#include "ltr/data/object_list.h"

namespace ltr {
  class Reporter {
  public:
    std::string report() const {
        return report_;
    }

    Reporter() {}
    explicit Reporter(const std::string& input)
        : report_(input) {}

    void clearReport() {
        report_.clear();
    }

    void appendReport(const std::string& input) {
        report_.append(input);
    }

    void setReport(const std::string& input) {
        report_ = input;
    }


  private:
    std::string report_;
  };


  class Aliaser {
  public:
    explicit Aliaser(const std::string& input)
        : alias_(input) {}

    std::string alias() const {
        return alias_;
    };

  private:
    const std::string alias_;
  };

  template <class TElement>
  struct Approach {
    static std::string name() {
      return "unknown";
    }
  };
  #define LISTWISE "listwise"
  #define POINTWISE "pointwise"
  #define PAIRWISE "pairwise"

  template<> std::string Approach<ltr::Object>::name();
  template<> std::string Approach<ltr::ObjectPair>::name();
  template<> std::string Approach<ltr::ObjectList>::name();
};

#endif  // LTR_INTERFACES_H_
