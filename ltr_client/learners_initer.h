// Copyright 2011 Yandex

#ifndef LTR_CLIENT_LEARNERS_INITER_H_
#define LTR_CLIENT_LEARNERS_INITER_H_

#include <map>
#include <string>
#include <vector>

#include "ltr_client/constants.h"
#include "ltr_client/learners_info.h"

#include "ltr/learners/best_feature_learner.h"

/**
@class LearnerIniter
This class is used to init learner of given type and approach.
*/
class LearnerIniter {
    private:
        template<class TElement>
        class Initer {
          typedef typename ltr::BaseLearner<TElement>::Ptr (*TFunct)
              (const LearnerInfo<TElement>&);
          TFunct function;

          public:
            Initer() {}
            typename ltr::BaseLearner<TElement>::Ptr operator()
                (const LearnerInfo<TElement>& info) {
              return function(info);
            }
            explicit Initer(TFunct function) : function(function) {}
        };
        map<std::string, map<std::string,
            boost::variant<Initer<ltr::Object>,
                           Initer<ltr::ObjectPair>,
                           Initer<ltr::ObjectList> > > >initers;
        map<std::string, vector<std::string> > approaches;

        /**
         * Function registers all learners in this factory.
         * Add your learners here
         */
        void registerLearners();

    public:
        LearnerIniter();
        /**
        Function creates learner object.
        @param info - information about learner to create.
        */
        template<class TElement>
        LearnerInfo<TElement> init(const LearnerInfo<TElement>& info);

        /**
        Function returns approach for learner with given type and approach.
        Throws if impossible.
        @param type - type of learner.
        @param approach - approach of learner, can be "" if unknown.
        */
        std::string getApproach(std::string type, std::string approach);

        template<class TElement>
        void registerLearner(std::string type,
            typename ltr::BaseLearner<TElement>::Ptr(*func)
                (const LearnerInfo<TElement>&)) {
          approaches[type].push_back(Approach<TElement>::name());
          initers[type][Approach<TElement>::name()] = Initer<TElement>(func);
        }
};

template<class TElement>
LearnerInfo<TElement> LearnerIniter::init(const LearnerInfo<TElement>& info) {
  typename ltr::BaseLearner<TElement>::Ptr learner_ptr =
        boost::get<Initer<TElement> >(initers[info.type][info.approach])(info);

  LearnerInfo<TElement> result = info;
  result.learner = learner_ptr;
  return result;
}

#define REGISTER_LISTWISE_LEARNER(type, function) \
  registerLearner<ltr::ObjectList>(type, function);

#define REGISTER_POINTWISE_LEARNER(type, function) \
  registerLearner<ltr::Object>(type, function);

#define REGISTER_PAIRWISE_LEARNER(type, function) \
  registerLearner<ltr::ObjectPair>(type, function);

#endif  // LTR_CLIENT_LEARNERS_INITER_H_
