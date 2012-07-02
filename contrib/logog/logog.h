#ifndef CONTRIB_LOGOG_LOGOG_H_
#define CONTRIB_LOGOG_LOGOG_H_

#include <boost/shared_ptr.hpp>

#include "contrib/logog/include/logog.hpp"
/*
namespace ltr {
  class Log {
  public:
    typedef boost::shared_ptr<logog::Target> TargetPtr;

    Log() {
      LOGOG_INITIALIZE();
      target = TargetPtr(new logog::Cout);
    }
    ~Log() {
      LOGOG_SHUTDOWN();
    }
  private:
    TargetPtr target;
  };
}

const ltr::Log LOG;
*/
#endif  // CONTRIB_LOGOG_LOGOG_H_