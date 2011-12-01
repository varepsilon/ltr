// Copyright 2011 Yandex

#ifndef LTR_INTERFACES_H_
#define LTR_INTERFACES_H_

#include <string>
#include <vector>

namespace ltr {

  class IReporter {
  public:
    /**
     * Instance's report. For example, report on completed work.
     */
    virtual std::string report() const = 0;

    virtual ~IReporter();
  };


  class IAliaser {
  public:
    /**
     * Instance's alias.
     */
    std::string alias() const {
      return alias_;
    };

    void setAlias(const std::string& alias) {
      alias_ = alias;
    }

  private:
    std::string alias_;
  };
};

#endif  // LTR_INTERFACES_H_
