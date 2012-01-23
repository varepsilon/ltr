// Copyright 2011 Yandex

#include "ltr_client/learners_initer.h"

#include <iostream>

#define ADD_INITER(type, element, function) \
    approaches[type].push_back(Approach<element>::name()); \
    initers[type][Approach<element>::name()] = \
                            LearnerIniter::Initer<element>(function);

LearnerIniter::LearnerIniter() {
  ADD_INITER("BEST_FEATURE", ltr::Object, BFIniter<ltr::Object>);
  ADD_INITER("BEST_FEATURE", ltr::ObjectPair, BFIniter<ltr::ObjectPair>);
  ADD_INITER("BEST_FEATURE", ltr::ObjectList, BFIniter<ltr::ObjectList>);
}

std::string LearnerIniter::getApproach(std::string type, std::string approach) {
    if (approaches.find(type) == approaches.end())
            throw std::logic_error("unknown learner type " + type);
    if (approach == "") {
        if (approaches[type].size() > 0)
            throw std::logic_error("can't determine approach for " + type);
        return approaches[type][0];
    } else {
        std::vector<string> v = approaches[type];
        if (std::find(v.begin(), v.end(), approach) != v.end())
            return approach;
        else
            throw std::logic_error(type + " learner can't be " + approach);
    }
    return "";
}
