
#include <boost/date_time/local_time/local_time.hpp>
#include <boost/lexical_cast.hpp>

#include "timer.h"

namespace timer {
  class Local {
  public:
    static string align(const string& str, size_t width) {
      if(width > str.size()) {
        return string(width-str.size(), '0') + str;
      } else {
        return str;
      }
    }
  };

  long long now() {
    using namespace boost::gregorian; 
    using namespace boost::posix_time;

    return (microsec_clock::universal_time() - ptime(date(1970,1,1))).total_milliseconds();
  }

  string formatTime() {
    string result;
    formatTime(result);
    return result;
  }

  void formatTime(string& to) {
    boost::posix_time::ptime currTime(boost::posix_time::second_clock::local_time());
    to =
      boost::lexical_cast< string >(currTime.date().year()) +
      "-" +
      Local::align(boost::lexical_cast< string >(currTime.date().month().as_number()),2) +
      "-" +
      Local::align(boost::lexical_cast< string >(currTime.date().day().as_number()),2) +
      " " +
      Local::align(boost::lexical_cast< string >(currTime.time_of_day().hours()),2) +
      ":" +
      Local::align(boost::lexical_cast< string >(currTime.time_of_day().minutes()),2) +
      ":" +
      Local::align(boost::lexical_cast< string >(currTime.time_of_day().seconds()),2)
      ;
  }

  Timer::Timer() :
  start_time_(now())
  {
  }

  long long Timer::startTime() const {
    return start_time_;
  }

  void Timer::reset(long long time) {
    start_time_ = time;
  }

  string Timer::operator()() const {
    long long time = now() - start_time_;
    string result;
    int sec_shift = 1;
    if(time > 60 * 1000) { // mins
      int min_shift = 1;
      if(time > 60 * 60 * 1000) { // hours
        int hours_shift = 1;
        if(time > 24 * 60 * 60 * 1000) { // days
          result += boost::lexical_cast< string >(time/(24*60*60*1000)) + " days ";
          hours_shift = 2;
        }
        result += Local::align(boost::lexical_cast< string >(time/(60*60*1000)%24), hours_shift) + " hours ";
        min_shift = 2;
      }
      result += Local::align(boost::lexical_cast< string >(time/(60*1000)%60), min_shift) + " minutes ";
      sec_shift = 2;
    }
    result += 
      Local::align(boost::lexical_cast< string >(time/1000%60), sec_shift) +
      " seconds " +
      Local::align(boost::lexical_cast< string >(time%1000), 3) +
      " millis";
    return result;
  };
};
