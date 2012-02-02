// Copyright 2012 Yandex

#include <string>
#include <stdexcept>
#include <map>
#include <vector>

#include "ltr/parameters_container/parameters_container.h"
#include "ltr_client/constants.h"

using std::string;
using ltr::ParametersContainer;

#ifndef LTR_CLIENT_FACTORY_H_
#define LTR_CLIENT_FACTORY_H_
/**
 * Class is used to create initers T
 */
template<class T>
class SimpleFactory {
  public:
    static typename T::BasePtr create
        (const ltr::ParametersContainer& parameters) {
      return typename T::BasePtr(new T(parameters));
    }
};
#endif  // LTR_CLIENT_FACTORY_H_

#if defined MEASURE_FACTORY
  #define FACTORY_CLASS MeasureFactory
  #define TObject ltr::Measure
#elif defined LEARNER_FACTORY
  #define FACTORY_CLASS LearnerFactory
  #define TObject ltr::BaseLearner
#elif defined SPLITTER_FACTORY
  #define FACTORY_CLASS MeasureFactory
  #define TObject ltr::cv::Splitter
#else
  #error "Unknown factory"
#endif

class FACTORY_CLASS {
  private:
    template<class TElement>
      class Initer {
        typedef typename TObject<TElement>::BasePtr (*TFunct)
            (const ParametersContainer&);
        TFunct function;

        public:
          Initer() {}
          typename TObject<TElement>::BasePtr operator()
              (const ParametersContainer& params) {
            return function(params);
          }
          explicit Initer(TFunct function) : function(function) {}
      };
      map<std::string, map<std::string,
            boost::variant<Initer<ltr::Object>,
                           Initer<ltr::ObjectPair>,
                           Initer<ltr::ObjectList> > > >initers;
      map<std::string, vector<std::string> > approaches;

      void registerAll();

  public:
    FACTORY_CLASS() {
      registerAll();
    }
    template<class TElement>
    typename TObject<TElement>::BasePtr init(string type,
        const ParametersContainer& parameters) {
      getApproach(type, Approach<TElement>::name());
      typename TObject<TElement>::Ptr object_ptr =
        boost::get<Initer<TElement> >
              (initers[type][Approach<TElement>::name()])(parameters);

      return object_ptr;
    }
    string getApproach(string type, string approach="") {
      if (approaches.find(type) == approaches.end())
        throw std::logic_error("unknown type " + type);
      if (approach == "") {
        if (approaches[type].size() > 1)
          throw std::logic_error("can't determine approach for " + type);
        return approaches[type][0];
      } else {
        std::vector<string> v = approaches[type];
        if (std::find(v.begin(), v.end(), approach) != v.end())
          return approach;
        else
          throw std::logic_error(type + " can't be " + approach);
      }
      return "";
    }

    template<class TElement>
    void registerClass(string type,
        typename TObject<TElement>::BasePtr(*func)
          (const ParametersContainer&)) {
      approaches[type].push_back(Approach<TElement>::name());
      initers[type][Approach<TElement>::name()] = Initer<TElement>(func);
    }
};
#undef FACTORY_CLASS
#undef TObject
