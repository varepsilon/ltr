// Copyright 2011 Yandex

#ifndef LTR_INTERFACES_ALIASER_H_
#define LTR_INTERFACES_ALIASER_H_

#include <string>

using std::string;

namespace ltr {
  /**
   * Aliaser holds alias of derived subclass
   * \deprecated
   */
  class Aliaser {
  public:
    explicit Aliaser(const string& input = "Alias")
      : alias_(input) {}

    string alias() const {
      return alias_;
    };

  private:
    string alias_;
  };
};

#endif  // LTR_INTERFACES_ALIASER_H_
