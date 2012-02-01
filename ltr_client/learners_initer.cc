// Copyright 2011 Yandex

#include "ltr_client/learners_initer.h"

#include <iostream>

LearnerIniter::LearnerIniter() {
  registerLearners();
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
