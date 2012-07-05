#ifndef CONTRIB_LOGOG_LOGOG_H_
#define CONTRIB_LOGOG_LOGOG_H_

#include <boost/shared_ptr.hpp>

#include "contrib/logog/include/logog.hpp"

namespace ltr {
  class Log {
  public:
    typedef boost::shared_ptr<logog::Target> TargetPtr;

    Log() {
      LOGOG_INITIALIZE();
      target_ = TargetPtr(new logog::Cout);
    }
    Log(const char* filename) {
      LOGOG_INITIALIZE();
      target_ = TargetPtr(new logog::LogFile(filename));
    }
    void setTargetAsFile(const char* filename) {
      target_ = TargetPtr(new logog::LogFile(filename));
    }
    void setTargetAsCout() {
      target_ = TargetPtr(new logog::Cout);
    }
    ~Log() {
      target_.reset();
      LOGOG_SHUTDOWN();
    }
  private:
    TargetPtr target_;
  };
};

#endif  // CONTRIB_LOGOG_LOGOG_H_