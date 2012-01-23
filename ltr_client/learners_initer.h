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
    public:
        LearnerIniter();
        /**
        Function creates learner object.
        @param info - information about learner to create.
        */
        template<class TElement>
        LearnerInfo<TElement> init(const LearnerInfo<TElement>& info);

        /**
        Function creates learner object.
        @param info - information about learner to create.
        @param weak_learner - information about weak_learner used by learner.
        */
        template<class TElement>
        LearnerInfo<TElement> init(const LearnerInfo<TElement>& info,
                                   const LearnerInfo<TElement>& weak_learner);

        /**
        Function returns approach for learner with given type and approach.
        Throws if impossible.
        @param type - type of learner.
        @param approach - approach of learner, can be "" if unknown.
        */
        std::string getApproach(std::string type, std::string approach);

    private:
        template<class TElement>
        class Initer {
          typedef LearnerInfo<TElement> (*TFunct)(const LearnerInfo<TElement>&,
                                                  const LearnerInfo<TElement>&);
          TFunct function;

          public:
            Initer() {}
            LearnerInfo<TElement> operator()(const LearnerInfo<TElement>& info,
                const LearnerInfo<TElement>& weak_ = LearnerInfo<TElement>()) {
              return function(info, weak_);
            }
            explicit Initer(TFunct function) : function(function) {}
        };
        map<std::string, map<std::string,
            boost::variant<Initer<ltr::Object>,
                           Initer<ltr::ObjectPair>,
                           Initer<ltr::ObjectList> > > >initers;
        map<std::string, vector<std::string> > approaches;

        VLearnerInfo no_info_;
};

template<class TElement>
LearnerInfo<TElement> LearnerIniter::init(const LearnerInfo<TElement>& info) {
    return boost::get<Initer<TElement> >(initers[info.type][info.approach])
                                                              (info);
}

template<class TElement>
LearnerInfo<TElement> LearnerIniter::init(const LearnerInfo<TElement>& info,
                                   const LearnerInfo<TElement>& weak_learner) {
    return boost::get<Initer<TElement> >(initers[info.type][info.approach])
                                                           (info, weak_learner);
}

template<class TElement>
LearnerInfo<TElement> BFIniter(const LearnerInfo<TElement>& info,
                               const LearnerInfo<TElement>& weak_info) {
  LearnerInfo<TElement> result = info;
  result.learner = typename ltr::BaseLearner<TElement>::Ptr(
                          new ltr::BestFeatureLearner<TElement>(info.measure));
  return result;
}

#endif  // LTR_CLIENT_LEARNERS_INITER_H_
