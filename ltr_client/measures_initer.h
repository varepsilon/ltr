// Copyright 2011 Yandex

#ifndef LTR_CLIENT_MEASURES_INITER_H_
#define LTR_CLIENT_MEASURES_INITER_H_

#include <string>
#include <map>

#include "ltr_client/constants.h"
#include "ltr_client/measures_info.h"

/**
@class MeasureIniter
This class is used to init measure of given type.
*/
class MeasureIniter {
    public:
        MeasureIniter();
        /**
        Function creates measure object.
        @param type - type of measure to create.
        @param parameters - parameters to create measure.
        */
        template<class TElement>
        MeasureInfo<TElement> init(const std::string& type,
                                   const ltr::ParametersContainer& parameters);

        /**
        Function returns approach for measure with given type.
        Throws if impossible.
        @param type - type of learner.
        */
        std::string getApproach(std::string type) {
          if (approach.find(type) == approach.end())
            throw std::logic_error("unknown measure type " + type);
          return approach[type];
        }

    private:
        /**
         * Function registers all measures in this factory.
         * Add your measures here
         */
        void registerMeasures();

        template<class TElement>
        class Initer {
          typedef typename ltr::Measure<TElement>::Ptr (*TFunct)
              (const ltr::ParametersContainer& parameters);
          TFunct function;

          public:
            Initer() {}
            typename ltr::Measure<TElement>::Ptr operator()
                (const ltr::ParametersContainer& parameters) {
              return function(parameters);
            }
            explicit Initer(TFunct function) : function(function) {}
        };
        map<std::string,
                boost::variant<Initer<ltr::Object>,
                           Initer<ltr::ObjectPair>,
                           Initer<ltr::ObjectList> > >initers;
        map<std::string, std::string> approach;
};

/**
 * Class is used to create initers for measures
 */
template<class TMeasure>
class SimpleMeasureFactory {
  public:
    static typename TMeasure::Ptr create
        (const ltr::ParametersContainer& parameters) {
      return typename TMeasure::Ptr(new TMeasure(parameters));
    }
};

template<class TElement>
MeasureInfo<TElement> MeasureIniter::init(const std::string& type,
    const ltr::ParametersContainer& parameters) {
  if (Approach<TElement>::name() != getApproach(type))
    throw std::logic_error("approach conflict in measure " + type);
  typename ltr::Measure<TElement>::Ptr measure_ptr =
        boost::get<Initer<TElement> >(initers[type])(parameters);

  MeasureInfo<TElement> result;
  result.measure = measure_ptr;
  result.type = type;
  result.approach = Approach<TElement>::name();
  return result;
}

#define REGISTER_LISTWISE_MEASURE(type, class_name) \
  initers[type] = MeasureIniter::Initer<ltr::ObjectList>(\
    SimpleMeasureFactory<class_name>::create); \
  approach[type] = Approach<ltr::ObjectList>::name();

#define REGISTER_POINTWISE_MEASURE(type, class_name) \
  initers[type] = MeasureIniter::Initer<ltr::Object>(\
    SimpleMeasureFactory<class_name>::create); \
  approach[type] = Approach<ltr::Object>::name();

#define REGISTER_PAIRWISE_MEASURE(type, class_name) \
  initers[type] = MeasureIniter::Initer<ltr::ObjectPair>(\
    SimpleMeasureFactory<class_name>::create); \
  approach[type] = Approach<ltr::ObjectPair>::name();

#endif  // LTR_CLIENT_MEASURES_INITER_H_
